#ifndef _MAP_H
#define _MAP_H

#include "GL.h"

#include <cglm/cglm.h>
#include "Blocks.h"

enum BLOCKS {
    AIR,
    BEDROCK,
    STONE,
    COBBLESTONE,
    DIRT,
    GRASS,
    BLOCK_COUNT,
};

struct Chunk {
    uint64_t* block_table;
    uint64_t block_render_counts[BLOCK_COUNT];
    uint64_t block_counts[BLOCK_COUNT];
    mat4* models[BLOCK_COUNT];
};

struct Map {
    struct Blocks* blocks;
    struct Chunk** chunks;
    uint64_t chunk_count;
    const char* map_path;
    vec3 player_position;
    float pitch, yaw;
};

struct GLOption map_load(const char* map_path);
struct Map* map_generate(const char* name, uint64_t seed);

void map_draw(const struct Map* map, const struct Shader* shader);
// void SaveMap(struct Map* map);
// void DestroyMap(struct Map* map);

#endif
