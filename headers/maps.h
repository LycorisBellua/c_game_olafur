#ifndef __MAPS_H__
#define __MAPS_H__

#include "coords.h"

typedef struct
{
    int width;
    int height;
    VectorF start_pos;
    float start_angle;
    int* data;
} Map;

#define MAP_CELL_LEN     64

extern Map* map_test;

void initialize_maps(void);
void set_minimap_display(const int remove_from_factor);
void draw_minimap(const Map* map);
void free_maps(void);

#endif

