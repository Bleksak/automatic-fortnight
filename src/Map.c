#include "Map.h"
#include <stdio.h>

#include <string.h>

static void closef(FILE** fp)
{
    if(*fp)
        fclose(*fp);
}

struct GLOption LoadMap(const char* map_path)
{
    __attribute__((cleanup(closef))) FILE* fp = fopen(map_path, "r");
    if(!fp)
    {
        return (struct GLOption)
        {
            .ok = false,
            .error_message = "Failed to load map",
        };
    }

    struct Map* map = calloc(1, sizeof(struct Map));

    char line[1024];
    
    unsigned int current_id = 0;

    while(fgets(line, 1024, fp))
    {
        char line_type[8];
        int offset;

        if(sscanf(line, "%s%n", line_type, &offset) != 1)
        {
            continue;
        }

        char* line_value = (line + offset + 1);

        switch(line_type[0])
        {
            case 'X':
            {
                sscanf(line_value, "%f", &map->PlayerPosition[0]);
            } break;

            case 'Y':
            {
                sscanf(line_value, "%f", &map->PlayerPosition[1]);
            } break;

            case 'Z':
            {
                sscanf(line_value, "%f", &map->PlayerPosition[2]);
            } break;

            case 'y':
            {
                sscanf(line_value, "%f", &map->yaw);
            } break;

            case 'p':
            {
                sscanf(line_value, "%f", &map->pitch);
            } break;

            case 'n':
            {
                unsigned int current_count;
                sscanf(line_value, "%u %u", &current_id, &current_count);
                map->blocks = realloc(map->blocks, (map->BlockCount + current_count) * sizeof(struct Block*));
            } break;

            case 'o':
            {
                strtok(line_value, "\n");

                struct GLOption objOption = LoadObjects(line_value);
                if(!objOption.ok)
                {
                    return objOption;
                }

                map->ObjectLibrary = realloc(map->ObjectLibrary, (map->ObjectLibLength + 1) * sizeof(struct Objects*));
                map->ObjectLibrary[map->ObjectLibLength++] = objOption.result_ptr;
            } break;

            case 'b':
            {
                vec3 position;
                sscanf(line_value, "%f %f %f", &position[0], &position[1], &position[2]);
                map->blocks[map->BlockCount++] = CreateBlock(position, map->ObjectLibrary[map->ObjectLibLength - 1], current_id);
            } break;
        }
    }

    return (struct GLOption)
    {
        .ok = true,
        .result_ptr = map,
    };
}

void DrawMap(struct Map* map, struct Shader* shader)
{
    for(unsigned long long i = 0; i < map->BlockCount; ++i)
    {
        DrawBlock(map->blocks[i], shader);
    }
}

void SaveMap(struct Map* map);
void DestroyMap(struct Map* map);