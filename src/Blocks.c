#include <string.h>
#include "Blocks.h"

void autoclose(FILE** fp)
{
    if(*fp)
        fclose(*fp);
}

static struct GLOption PushTex(char* texture_names[6])
{
    struct GLOption texOpt = LoadBlockTextures(texture_names);
    for(unsigned int i = 0; i < 6; ++i)
        free(texture_names[i]);
    
    return texOpt;
}

struct GLOption LoadBlocks(const char* file)
{
    __attribute__((cleanup(autoclose))) FILE* fp = fopen(file, "rb");
    if(!fp)
    {
        return (struct GLOption)
        {
            .ok = false,
            .error_message = "Failed to load objects",
        };
    }

    struct Blocks* blocks = calloc(1, sizeof(struct Blocks));
    blocks->textures = calloc(1, sizeof(GLuint));
    char line[1024];
    char* texture_names[6] = {0};

    while(fgets(line, 1024, fp))
    {
        char line_type[8];
        int offset;

        if(sscanf(line, "%s%n", line_type, &offset) != 1)
        {
            continue;
        }

        char* line_value = line + offset + 1;

        if(strcmp(line_type, "id") == 0)
        {
            if(blocks->length)
            {
                struct GLOption texOpt = PushTex(texture_names);
                if(!texOpt.ok)
                {
                    return texOpt;
                }

                blocks->textures[blocks->length - 1] = texOpt.result_gluint;
            }

            blocks->length++;

            blocks->ids = realloc(blocks->ids, sizeof(unsigned long long) * blocks->length);
            blocks->names = realloc(blocks->names, sizeof(char*) * blocks->length);
            blocks->textures = realloc(blocks->textures, sizeof(GLuint*) * blocks->length);
            sscanf(line_value, "%llu", &blocks->ids[blocks->length - 1]);
        }
        
        else if(strcmp(line_type, "name") == 0)
        {
            strtok(line_value, "\n");
            blocks->names[blocks->length - 1] = calloc(strlen(line_value) + 1, sizeof(char));
            strncpy(blocks->names[blocks->length - 1], line_value, strlen(line_value));
        }

        else if(strcmp(line_type, "right") == 0)
        {
            strtok(line_value, "\n");
            texture_names[0] = calloc(strlen(line_value) + 1, sizeof(char));
            strncpy(texture_names[0], line_value, strlen(line_value));
        }

        else if(strcmp(line_type, "left") == 0)
        {
            strtok(line_value, "\n");
            texture_names[1] = calloc(strlen(line_value) + 1, sizeof(char));
            strncpy(texture_names[1], line_value, strlen(line_value));
        }

        else if(strcmp(line_type, "top") == 0)
        {
            strtok(line_value, "\n");
            texture_names[2] = calloc(strlen(line_value) + 1, sizeof(char));
            strncpy(texture_names[2], line_value, strlen(line_value));
        }

        else if(strcmp(line_type, "bot") == 0)
        {
            strtok(line_value, "\n");
            texture_names[3] = calloc(strlen(line_value) + 1, sizeof(char));
            strncpy(texture_names[3], line_value, strlen(line_value));
        }

        else if(strcmp(line_type, "front") == 0)
        {
            strtok(line_value, "\n");
            texture_names[4] = calloc(strlen(line_value) + 1, sizeof(char));
            strncpy(texture_names[4], line_value, strlen(line_value));
        }
        
        else if(strcmp(line_type, "back") == 0)
        {
            strtok(line_value, "\n");
            texture_names[5] = calloc(strlen(line_value) + 1, sizeof(char));
            strncpy(texture_names[5], line_value, strlen(line_value));
        }
    }

    struct GLOption texOpt = PushTex(texture_names);
    if(!texOpt.ok)
    {
        return texOpt;
    }

    blocks->textures[blocks->length - 1] = texOpt.result_gluint;

    blocks->length++;

    glGenVertexArrays(1, &blocks->vao);
    glBindVertexArray(blocks->vao);

    glGenBuffers(1, &blocks->vbo);
    glGenBuffers(1, &blocks->ebo);
    // glGenBuffers(1, &blocks->vbo_textures);

    float vertices[] = 
    {
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
    };

    // float tex[] = 
    // {
    //     1.0f, 0.0f,
    //     0.0f, 1.0f,
    //     0.0f, 0.0f,
    //     1.0f, 0.0f,
    //     0.0f, 1.0f,
    //     0.0f, 0.0f,
    //     1.0f, 0.0f,
    //     0.0f, 1.0f,
    //     0.0f, 0.0f,
    //     1.0f, 0.0f,
    //     0.0f, 1.0f,
    //     0.0f, 0.0f,
    //     1.0f, 0.0f,
    //     0.0f, 1.0f,
    //     0.0f, 0.0f,
    //     1.0f, 0.0f,
    //     0.0f, 1.0f,
    //     0.0f, 0.0f,
    //     1.0f, 0.0f,
    //     1.0f, 1.0f,
    //     0.0f, 1.0f,
    //     1.0f, 0.0f,
    //     1.0f, 1.0f,
    //     0.0f, 1.0f,
    //     1.0f, 0.0f,
    //     1.0f, 1.0f,
    //     0.0f, 1.0f,
    //     1.0f, 0.0f,
    //     1.0f, 1.0f,
    //     0.0f, 1.0f,
    //     1.0f, 0.0f,
    //     1.0f, 1.0f,
    //     0.0f, 1.0f,
    //     1.0f, 0.0f,
    //     1.0f, 1.0f,
    //     0.0f, 1.0f,
    // };

    unsigned int indices[] = 
    {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35,
    };

    blocks->drawCount = sizeof(indices) / sizeof(indices[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, blocks->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, blocks->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, blocks->vbo_textures);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
    
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(1);

    return (struct GLOption)
    {
        .ok = true,
        .result_ptr = blocks,
    };
}

void DrawBlock(struct Blocks* blocks, unsigned int id)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, blocks->textures[id]);
    glBindVertexArray(blocks->vao);
    glDrawElements(GL_TRIANGLES, blocks->drawCount, GL_UNSIGNED_INT, 0);
}