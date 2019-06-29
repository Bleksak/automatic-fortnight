#ifndef _MAP_H
#define _MAP_H

#include "GL.h"
#include "Block.h"
#include "Objects.h"

#include "include/cglm/cglm.h"

struct Map
{
    const char* map_path;
    vec3 PlayerPosition;
    float pitch, yaw;
    struct Block** blocks;
    struct Objects** ObjectLibrary;
    unsigned long long ObjectLibLength;
    unsigned long long BlockCount;
};

struct GLOption LoadMap(const char* map_path);
void DrawMap(struct Map* map, struct Shader* shader);
void SaveMap(struct Map* map);
void DestroyMap(struct Map* map);

#endif