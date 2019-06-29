#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Objects.h"
#include "GL.h"

struct TemporaryData
{
    vec3* v;
    vec3* vn;
    vec2* vt;

    unsigned int* f_vertex;
    unsigned int* f_normal;
    unsigned int* f_texture;

    unsigned int v_size;
    unsigned int vn_size;
    unsigned int vt_size;
};

static void closef(FILE **fp)
{
    if(*fp)
        fclose(*fp);
}

static void pushObject(struct Objects* objects, struct TemporaryData* data)
{
    if(objects->length > 0)
    {
        objects->objects[objects->length - 1].v  = malloc(objects->objects[objects->length - 1].f_size * sizeof(vec3));
        objects->objects[objects->length - 1].vt = malloc(objects->objects[objects->length - 1].f_size * sizeof(vec2));
        objects->objects[objects->length - 1].vn = malloc(objects->objects[objects->length - 1].f_size * sizeof(vec3));
        objects->objects[objects->length - 1].f  = malloc(objects->objects[objects->length - 1].f_size * sizeof(unsigned int));

        for(unsigned int i = 0; i < objects->objects[objects->length - 1].f_size; ++i)
        {
            unsigned int vertex_idx = data->f_vertex[i]  - 1;
            unsigned int tex_idx    = data->f_texture[i] - 1;
            unsigned int normal_idx = data->f_normal[i]  - 1;

            memcpy(objects->objects[objects->length - 1].vt[i], data->vt[tex_idx], sizeof(vec2));
            glm_vec3_copy(data->v[vertex_idx], objects->objects[objects->length - 1].v[i]);
            glm_vec3_copy(data->vn[normal_idx], objects->objects[objects->length - 1].vn[i]);

            objects->objects[objects->length - 1].f[i] = i;
        }

        free(data->v);
        free(data->vt);
        free(data->vn);
        free(data->f_vertex);
        free(data->f_normal);
        free(data->f_texture);

        memset(data, 0, sizeof(struct TemporaryData));
    }
    else
    {
        objects->objects = realloc(objects->objects, sizeof(struct Object) * (objects->length + 1));
        memset(&objects->objects[objects->length], 0, sizeof(struct Object));
    }
    
    objects->length++;
}

struct GLOption LoadObjects(const char* filename)
{
    __attribute__((cleanup(closef))) FILE* fp = fopen(filename, "rb");
    if(!fp)
    {
        return (struct GLOption)
        {
            .ok = false,
            .error_message = "Failed to load objects",
        };
    }

    struct Objects* objects = calloc(1, sizeof(struct Objects));
    
    struct TemporaryData data = {0};

    char line[1024];
    
    while(fgets(line, 1024, fp))
    {
        int offset;
        char line_type[8];
        
        if(sscanf(line, "%s%n", line_type, &offset) != 1)
        {
            continue;
        }

        char* line_value = (line + offset + 1);

        switch(line_type[0])
        {
            case 'm':
            {
                strtok(line_value, "\n");
                struct GLOption option = LoadTextures(line_value);

                if(!option.ok)
                {
                    free(objects);
                    return option;
                }

                objects->textures = option.result_ptr;
            } break;

            case 'n':
            {
                // printf("SIZE: %llu\n", objects->length);
                pushObject(objects, &data);

                objects->objects = realloc(objects->objects, sizeof(struct Object) * objects->length);

                memset(&objects->objects[objects->length - 1], 0 , sizeof(struct Object));
            } break;

            case 'v':
            {
                switch(line_type[1])
                {
                    case 't':
                    {
                        data.vt = realloc(data.vt, (data.vt_size + 1) * sizeof(vec2));
                        sscanf(line_value, "%f %f", &data.vt[data.vt_size][0], &data.vt[data.vt_size][1]);
                        data.vt_size++;
                    } break;

                    case 'n':
                    {
                        data.vn = realloc(data.vn, (data.vn_size + 1) * sizeof(vec3));
                        sscanf(line_value, "%f %f %f", &data.vn[data.vn_size][0], &data.vn[data.vn_size][1], &data.vn[data.vn_size][2]);
                        data.vn_size++;
                    } break;

                    default:
                    {
                        data.v = realloc(data.v, (data.v_size + 1) * sizeof(vec3));
                        sscanf(line_value, "%f %f %f", &data.v[data.v_size][0], &data.v[data.v_size][1], &data.v[data.v_size][2]);
                        data.v_size++;
                    } break;
                }
            } break;

            case 'u':
            {
                objects->objects[objects->length - 1].materials.materials = realloc(objects->objects[objects->length - 1].materials.materials, (objects->objects[objects->length - 1].materials.length + 1) * sizeof(struct Material));
                objects->objects[objects->length - 1].materials.materials[objects->objects[objects->length - 1].materials.length].block_count = 0;

                sscanf(line_value, "%u", &objects->objects[objects->length - 1].materials.materials[objects->objects[objects->length - 1].materials.length].id);
                
                objects->objects[objects->length - 1].materials.length++;
            } break;

            case 'f':
            {
                data.f_vertex  = realloc(data.f_vertex,  (objects->objects[objects->length - 1].f_size + 3) * sizeof(unsigned int));
                data.f_normal  = realloc(data.f_normal,  (objects->objects[objects->length - 1].f_size + 3) * sizeof(unsigned int));
                data.f_texture = realloc(data.f_texture, (objects->objects[objects->length - 1].f_size + 3) * sizeof(unsigned int));

                sscanf(line_value, "%u/%u/%u %u/%u/%u %u/%u/%u",
                    &data.f_vertex[objects->objects[objects->length - 1].f_size    ], &data.f_texture[objects->objects[objects->length - 1].f_size    ], &data.f_normal[objects->objects[objects->length - 1].f_size    ],
                    &data.f_vertex[objects->objects[objects->length - 1].f_size + 1], &data.f_texture[objects->objects[objects->length - 1].f_size + 1], &data.f_normal[objects->objects[objects->length - 1].f_size + 1],
                    &data.f_vertex[objects->objects[objects->length - 1].f_size + 2], &data.f_texture[objects->objects[objects->length - 1].f_size + 2], &data.f_normal[objects->objects[objects->length - 1].f_size + 2]
                );

                objects->objects[objects->length - 1].materials.materials[objects->objects[objects->length - 1].materials.length - 1].block_count += 3; // maybe add -1 on material.length
                objects->objects[objects->length - 1].f_size += 3;
            } break;
        }
    }

    pushObject(objects, &data);
    
    return (struct GLOption)
    {
        .ok = true,
        .result_ptr = objects,
    };
}

// void UnloadObjects(struct Objects* objects)
// {

// }