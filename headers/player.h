#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "coords.h"
#include "maps.h"

typedef struct
{
    VectorF pos;
    VectorF delta;
    float angle;
    int map_index;
} Player;

extern Player player;

void reset_player_transform(const Map* m);
void update_player_transform(const Map* m);

#endif

