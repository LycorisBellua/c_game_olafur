#ifndef __MAPS_H__
#define __MAPS_H__

#include "coords.h"

typedef struct
{
    int width;
    int height;
    int cell;
    int* data;
} Map;

typedef struct
{
    VectorF pos;
    VectorF delta;
    float angle;
} Player;

extern Map* map_test;
extern Player player;

Map* create_map(void);
void draw_minimap(const Map* map);
void free_map(Map** map);

#endif

