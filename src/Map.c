#include "Map.h"
#include <stdio.h>

#include <string.h>
#include <math.h>

static void closef(FILE** fp) {
    if(*fp)
        fclose(*fp);
}

struct GenBlockPosition {
    uint32_t block_id;
    uint32_t maxY, minY;
};

static struct GenBlockPosition blocks[] =  {
    {GRASS, 65, 59},
    {STONE, 58, 1},
};

struct Vector2D {int64_t x; int64_t y;};

static struct Vector2D spiral_get_coords(int64_t i) {
    int64_t index = i + 1;
    int64_t s = ((int64_t)ceil(sqrtf((double)index)) + (((int64_t)(ceil(sqrtf((double)index))) % 2 + 1) % 2));
    int64_t ringIndex = 0;
    int64_t p = 1;
    if (s > 1) {
        ringIndex = i - (s - 2) * (s - 2);
        p = s * s - (s - 2) * (s - 2);
    }

    int64_t ri = (ringIndex + (int64_t) (s / 2)) % p;

    int64_t x = 0;
    if (s > 1)
        x = ri < (p / 4) ? ri :
                (ri <= (p / 4 * 2 - 1) ? (p / 4) :
                        (ri <= (p / 4 * 3) ? ((p / 4 * 3) - ri) :
                                0));

    int64_t y = 0;
    if (s > 1)
        y = ri < (p / 4) ? 0 :
                (ri <= (p / 4 * 2 - 1) ? (ri - (p / 4)) :
                        (ri <= (p / 4 * 3) ? (p / 4) :
                                (p - ri)));

    x -= (int64_t) (s / 2);
    y -= (int64_t) (s / 2);

    return (struct Vector2D){x, y};
}

static void set_block_visible(struct Chunk* chunk, uint64_t x, uint64_t y, uint64_t z, struct Vector2D* dXY)
{
    uint32_t the_block = chunk->block_table[x * 128 * 16 + y * 16 + z];

    chunk->models[the_block] = realloc(chunk->models[the_block], sizeof(mat4) * (chunk->block_render_counts[the_block] + 1));
    glm_mat4_identity(chunk->models[the_block][chunk->block_render_counts[the_block]]);
    glm_translate(chunk->models[the_block][chunk->block_render_counts[the_block]], (vec3){(float) (dXY->x * 16 + (int64_t)x), (float) y, (float) (dXY->y * 16 + (int64_t)z)});
    chunk->block_render_counts[the_block]++;
}

static struct Chunk* chunk_generate(const struct Map* map, uint64_t seed, int64_t chunk_id)
{
    struct Chunk* chunk = calloc(1, sizeof(struct Chunk));
    srand(seed);

    struct Vector2D dXY = spiral_get_coords(chunk_id);

    chunk->block_table = calloc(16 * 16 * 128, sizeof(uint64_t));

    for(uint32_t x = 0; x < 16; ++x)
    {
        for(uint32_t z = 0; z < 16; ++z)
        {
            for(uint32_t y = 0; y < 128; ++y)
            {
                uint32_t possible_blocks[BLOCK_COUNT] = {0};
                uint32_t possible_block_count = 0;

                for(uint32_t i = 0; i < sizeof(blocks) / sizeof(blocks[0]); ++i)
                {
                    if(y > blocks[i].maxY)
                        continue;
                    if(y < blocks[i].minY)
                        continue;
                    
                    possible_blocks[possible_block_count++] = blocks[i].block_id;
                }

                if(!possible_block_count)
                    continue;

                uint32_t the_block = possible_blocks[ ( chunk_id * (seed * rand() + rand() + (x * 16 * 16 + z * 16 + y))) % possible_block_count ];
                
                chunk->block_table[x * 128 * 16 + y * 16 + z] = the_block;
                chunk->block_counts[the_block]++;
            }
        }
    }


    for(uint32_t x = 0; x < 16; ++x)
    {
        for(uint32_t z = 0; z < 16; ++z)
        {
            for(uint32_t y = 0; y < 127; ++y)
            {
                if(y > 0 && chunk->block_table[x * 128 * 16 + (y-1) * 16 + z] == AIR)
                {
                    set_block_visible(chunk, x, y, z, &dXY);
                }
                else if(y < 127 && chunk->block_table[x * 128 * 16 + (y+1) * 16 + z] == AIR)
                {
                    set_block_visible(chunk, x, y, z, &dXY);
                }
                else if(z > 0 && chunk->block_table[x * 128 * 16 + y * 16 + z - 1] == AIR)
                {
                    set_block_visible(chunk, x, y, z, &dXY);
                }
                else if(z < 15 && chunk->block_table[x * 128 * 16 + y * 16 + z + 1] == AIR)
                {
                    set_block_visible(chunk, x, y, z, &dXY);
                }
                else if(x > 0 && chunk->block_table[(x-1) * 128 * 16 + y * 16 + z] == AIR)
                {
                    set_block_visible(chunk, x, y, z, &dXY);
                }
                else if(x < 15 && chunk->block_table[(x+1) * 128 * 16 + y * 16 + z] == AIR)
                {
                    set_block_visible(chunk, x, y, z, &dXY);
                }
                else if(x == 0 || x == 15 || z == 0 || z == 15)
                {
                    // TODO: Add connection between 2 and more chunks, and erase sides where needed
                    
                    set_block_visible(chunk, x, y, z, &dXY);
                }
            }
        }
    }

    return chunk;
}

struct Map* map_generate(const char* name, uint64_t seed)
{
    // limit to a few chunks for now
    
    struct Map* map = calloc(1, sizeof(struct Map));
    map->blocks = unwrap(void*, blocks_load("blocks/objects.atf"));
    
    for(uint32_t i = 0; i < 4; ++i)
    {
        map->chunks = realloc(map->chunks, sizeof(struct Chunk*) * (map->chunk_count + 1));
        map->chunks[map->chunk_count] = chunk_generate(map, seed, map->chunk_count);
        map->chunk_count++;
    }

    return map;
}

void map_draw(const struct Map* map, const struct Shader* shader)
{
    glBindVertexArray(map->blocks->vao);

    for(uint64_t c = 0; c < map->chunk_count; ++c)
    {
        for(uint64_t block_id = 1; block_id < BLOCK_COUNT; ++block_id) // zero is air
        {
            if(!map->chunks[c]->block_render_counts[block_id])
                continue;

            glBindTexture(GL_TEXTURE_CUBE_MAP, map->blocks->textures[block_id - 1]);
            for(uint32_t i = 0; i < map->chunks[c]->block_render_counts[block_id]; ++i)
            {
                glUniformMatrix4fv(shader->model_position, 1, GL_FALSE, &map->chunks[c]->models[block_id][i][0][0]);
                glDrawElements(GL_TRIANGLES, map->blocks->draw_count, GL_UNSIGNED_INT, 0);
            }
        }
    }
}

void map_save(struct Map* map);
void map_destroy(struct Map* map);
