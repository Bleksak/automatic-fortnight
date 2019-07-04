#ifndef _MAP_H
#define _MAP_H

#include "GL.h"

#include "include/cglm/cglm.h"

// struct Chunk
// {
//     unsigned long long block_counts[BLOCK_COUNT];
//     mat4* models[BLOCK_COUNT];
// };

// struct Map
// {
//     struct Blocks blocks;
//     struct Chunk** chunks;
//     unsigned long long chunkCount;
//     const char* map_path;
//     vec3 PlayerPosition;
//     float pitch, yaw;
//     unsigned long long BlockCount;
// };

struct GLOption LoadMap(const char* map_path);
// struct Map* GenerateMap(const char* name, unsigned long long seed);

// void DrawMap(const struct Map* map, const struct Shader* shader);
// void SaveMap(struct Map* map);
// void DestroyMap(struct Map* map);

#endif