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

typedef struct
{
    VectorF pos;
    VectorF delta;
    float angle;
} Player;

#define MINIMAP_ZOOM_MIN 1
#define MINIMAP_ZOOM_MAX 5

extern Map* map_test;
extern Player player;
extern int minimap_zoom;
extern const int cell_len;

void initialize_maps(void);
void set_minimap_display(const double zoom_level);
void draw_minimap(const Map* map);
void free_maps(void);

#endif

