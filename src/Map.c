#include "Map.h"
#include <stdio.h>

#include <string.h>
#include <math.h>

static void closef(FILE** fp)
{
    if(*fp)
        fclose(*fp);
}

// struct GLOption LoadMap(const char* map_path)
// {
//     __attribute__((cleanup(closef))) FILE* fp = fopen(map_path, "r");
//     if(!fp)
//     {
//         return (struct GLOption)
//         {
//             .ok = false,
//             .error_message = "Failed to load map",
//         };
//     }

//     struct Map* map = calloc(1, sizeof(struct Map));
    
//     char line[1024];
    
//     unsigned int current_id = 0;

//     while(fgets(line, 1024, fp))
//     {
//         char line_type[8];
//         int offset;

//         if(sscanf(line, "%s%n", line_type, &offset) != 1)
//         {
//             continue;
//         }

//         char* line_value = (line + offset + 1);

//         switch(line_type[0])
//         {
//             case 'X':
//             {
//                 sscanf(line_value, "%f", &map->PlayerPosition[0]);
//             } break;

//             case 'Y':
//             {
//                 sscanf(line_value, "%f", &map->PlayerPosition[1]);
//             } break;

//             case 'Z':
//             {
//                 sscanf(line_value, "%f", &map->PlayerPosition[2]);
//             } break;

//             case 'y':
//             {
//                 sscanf(line_value, "%f", &map->yaw);
//             } break;

//             case 'p':
//             {
//                 sscanf(line_value, "%f", &map->pitch);
//             } break;

//             case 'n':
//             {
//                 unsigned int current_count;
//                 sscanf(line_value, "%u %u", &current_id, &current_count);
//                 map->blocks = realloc(map->blocks, (map->BlockCount + current_count) * sizeof(struct Block*));
//             } break;

//             case 'o':
//             {
//                 strtok(line_value, "\n");

//                 struct GLOption objOption = LoadObjects(line_value);
//                 if(!objOption.ok)
//                 {
//                     return objOption;
//                 }

//                 map->ObjectLibrary = realloc(map->ObjectLibrary, (map->ObjectLibLength + 1) * sizeof(struct Objects*));
//                 map->ObjectLibrary[map->ObjectLibLength++] = objOption.result_ptr;
//             } break;

//             case 'b':
//             {
//                 vec3 position;
//                 sscanf(line_value, "%f %f %f", &position[0], &position[1], &position[2]);
//                 map->blocks[map->BlockCount++] = CreateBlock(position, map->ObjectLibrary[map->ObjectLibLength - 1], current_id);
//             } break;
//         }
//     }

//     return (struct GLOption)
//     {
//         .ok = true,
//         .result_ptr = map,
//     };
// }


struct GenBlockPosition
{
    unsigned int block_id;
    unsigned int maxY, minY;
};

static struct GenBlockPosition blocks[] = 
{
    {GRASS, 65, 59},
    {STONE, 58, 1},
};

struct Vector2D {long long x; long long y;};

static struct Vector2D getSpiralCoords(long long i) {
    long long index = i + 1;
    long long s = ((long long)ceil(sqrtf((double)index)) + (((long long)(ceil(sqrtf((double)index))) % 2 + 1) % 2));
    long long ringIndex = 0;
    long long p = 1;
    if (s > 1) {
        ringIndex = i - (s - 2) * (s - 2);
        p = s * s - (s - 2) * (s - 2);
    }

    long long ri = (ringIndex + (long long) (s / 2)) % p;

    long long x = 0;
    if (s > 1)
        x = ri < (p / 4) ? ri :
                (ri <= (p / 4 * 2 - 1) ? (p / 4) :
                        (ri <= (p / 4 * 3) ? ((p / 4 * 3) - ri) :
                                0));

    long long y = 0;
    if (s > 1)
        y = ri < (p / 4) ? 0 :
                (ri <= (p / 4 * 2 - 1) ? (ri - (p / 4)) :
                        (ri <= (p / 4 * 3) ? (p / 4) :
                                (p - ri)));

    x -= (long long) (s / 2);
    y -= (long long) (s / 2);

    return (struct Vector2D){x, y};
}

static void SetBlockVisible(struct Chunk* chunk, unsigned long long x, unsigned long long y, unsigned long long z, struct Vector2D* dXY)
{
    unsigned int the_block = chunk->block_table[x * 128 * 16 + y * 16 + z];

    chunk->models[the_block] = realloc(chunk->models[the_block], sizeof(mat4) * (chunk->block_render_counts[the_block] + 1));
    glm_mat4_identity(chunk->models[the_block][chunk->block_render_counts[the_block]]);
    glm_translate(chunk->models[the_block][chunk->block_render_counts[the_block]], (vec3){(float) (dXY->x * 16 + (long long)x), (float) y, (float) (dXY->y * 16 + (long long)z)});
    chunk->block_render_counts[the_block]++;
}

static struct Chunk* GenerateChunk(const struct Map* map, unsigned long long seed, long long chunk_id)
{
    struct Chunk* chunk = calloc(1, sizeof(struct Chunk));
    srand(seed);

    struct Vector2D dXY = getSpiralCoords(chunk_id);

    chunk->block_table = calloc(16 * 16 * 128, sizeof(unsigned long long));

    for(unsigned int x = 0; x < 16; ++x)
    {
        for(unsigned int z = 0; z < 16; ++z)
        {
            for(unsigned int y = 0; y < 128; ++y)
            {
                unsigned int possible_blocks[BLOCK_COUNT] = {0};
                unsigned int possible_block_count = 0;

                for(unsigned int i = 0; i < sizeof(blocks) / sizeof(blocks[0]); ++i)
                {
                    if(y > blocks[i].maxY)
                        continue;
                    if(y < blocks[i].minY)
                        continue;
                    
                    possible_blocks[possible_block_count++] = blocks[i].block_id;
                }

                if(!possible_block_count)
                    continue;

                unsigned int the_block = possible_blocks[ ( chunk_id * (seed * rand() + rand() + (x * 16 * 16 + z * 16 + y))) % possible_block_count ];
                
                chunk->block_table[x * 128 * 16 + y * 16 + z] = the_block;
                chunk->block_counts[the_block]++;
            }
        }
    }


    for(unsigned int x = 0; x < 16; ++x)
    {
        for(unsigned int z = 0; z < 16; ++z)
        {
            for(unsigned int y = 0; y < 127; ++y)
            {
                if(y > 0 && chunk->block_table[x * 128 * 16 + (y-1) * 16 + z] == AIR)
                {
                    SetBlockVisible(chunk, x, y, z, &dXY);
                }
                else if(y < 127 && chunk->block_table[x * 128 * 16 + (y+1) * 16 + z] == AIR)
                {
                    SetBlockVisible(chunk, x, y, z, &dXY);
                }
                else if(z > 0 && chunk->block_table[x * 128 * 16 + y * 16 + z - 1] == AIR)
                {
                    SetBlockVisible(chunk, x, y, z, &dXY);
                }
                else if(z < 15 && chunk->block_table[x * 128 * 16 + y * 16 + z + 1] == AIR)
                {
                    SetBlockVisible(chunk, x, y, z, &dXY);
                }
                else if(x > 0 && chunk->block_table[(x-1) * 128 * 16 + y * 16 + z] == AIR)
                {
                    SetBlockVisible(chunk, x, y, z, &dXY);
                }
                else if(x < 15 && chunk->block_table[(x+1) * 128 * 16 + y * 16 + z] == AIR)
                {
                    SetBlockVisible(chunk, x, y, z, &dXY);
                }
                else if(x == 0 || x == 15 || z == 0 || z == 15)
                {
                    // TODO: Add connection between 2 and more chunks, and erase sides where needed
                    
                    SetBlockVisible(chunk, x, y, z, &dXY);
                }
            }
        }
    }

    return chunk;
}

struct Map* GenerateMap(const char* name, unsigned long long seed)
{
    // limit to a few chunks for now
    
    struct Map* map = calloc(1, sizeof(struct Map));
    map->blocks = LoadBlocks("blocks/objects.atf").result_ptr;
    
    for(unsigned int i = 0; i < 4; ++i)
    {
        map->chunks = realloc(map->chunks, sizeof(struct Chunk*) * (map->chunkCount + 1));
        map->chunks[map->chunkCount] = GenerateChunk(map, seed, map->chunkCount);
        map->chunkCount++;
    }

    return map;
}

void DrawMap(const struct Map* map, const struct Shader* shader)
{
    glBindVertexArray(map->blocks->vao);

    for(unsigned long long c = 0; c < map->chunkCount; ++c)
    {
        for(unsigned long long block_id = 1; block_id < BLOCK_COUNT; ++block_id) // zero is air
        {
            if(!map->chunks[c]->block_render_counts[block_id])
                continue;

            glBindTexture(GL_TEXTURE_CUBE_MAP, map->blocks->textures[block_id - 1]);
            for(unsigned int i = 0; i < map->chunks[c]->block_render_counts[block_id]; ++i)
            {
                glUniformMatrix4fv(shader->model_position, 1, GL_FALSE, &map->chunks[c]->models[block_id][i][0][0]);
                glDrawElements(GL_TRIANGLES, map->blocks->drawCount, GL_UNSIGNED_INT, 0);
            }
        }
    }
}

void SaveMap(struct Map* map);
void DestroyMap(struct Map* map);