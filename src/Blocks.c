#include <string.h>
#include "Blocks.h"

void autoclose(FILE** fp)
{
    if(*fp)
        fclose(*fp);
}

static void PushTex(struct Blocks* blocks, struct TexImageStrData* texData)
{
    LoadBlockTextures(&blocks->textures[blocks->length - 1], texData);
    free(texData->PositiveX);
    free(texData->PositiveY);
    free(texData->PositiveZ);
    free(texData->NegativeX);
    free(texData->NegativeY);
    free(texData->NegativeZ);
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
    struct TexImageStrData texData = {0};

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
                PushTex(blocks, &texData);
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
            texData.PositiveX = calloc(strlen(line_value) + 1, sizeof(char));
            strncpy(texData.PositiveX, line_value, strlen(line_value));
        }

        else if(strcmp(line_type, "left") == 0)
        {
            strtok(line_value, "\n");
            texData.NegativeX = calloc(strlen(line_value) + 1, sizeof(char));
            strncpy(texData.NegativeX, line_value, strlen(line_value));
        }

        else if(strcmp(line_type, "top") == 0)
        {
            strtok(line_value, "\n");
            texData.PositiveY = calloc(strlen(line_value) + 1, sizeof(char));
            strncpy(texData.PositiveY, line_value, strlen(line_value));
        }

        else if(strcmp(line_type, "bot") == 0)
        {
            strtok(line_value, "\n");
            texData.NegativeY = calloc(strlen(line_value) + 1, sizeof(char));
            strncpy(texData.NegativeY, line_value, strlen(line_value));
        }

        else if(strcmp(line_type, "front") == 0)
        {
            strtok(line_value, "\n");
            texData.PositiveZ = calloc(strlen(line_value) + 1, sizeof(char));
            strncpy(texData.PositiveZ, line_value, strlen(line_value));
        }
        
        else if(strcmp(line_type, "back") == 0)
        {
            strtok(line_value, "\n");
            texData.NegativeZ = calloc(strlen(line_value) + 1, sizeof(char));
            strncpy(texData.NegativeZ, line_value, strlen(line_value));
        }
    }

    PushTex(blocks, &texData);
    blocks->length++;

    glGenVertexArrays(1, &blocks->vao);
    glBindVertexArray(blocks->vao);

    glGenBuffers(1, &blocks->vbo);
    glGenBuffers(1, &blocks->ebo);
    glGenBuffers(1, &blocks->vbo_textures);

    float vertices[] = 
    {
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
    };

    float tex[] = 
    {
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
    };

    unsigned int indices[] = 
    {
        1, 2, 0,
        3, 6, 2,
        7, 4, 6,
        5, 0, 4,
        6, 0, 2,
        3, 5, 7,
        1, 3, 2,
        3, 7, 6,
        7, 5, 4,
        5, 1, 0,
        6, 4, 0,
        3, 1, 5,
    };

    return (struct GLOption)
    {
        .ok = true,
        .result_ptr = blocks,
    };
}