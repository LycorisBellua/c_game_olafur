#include "cimmerian.h"

static void close_last_door(t_map* m, t_list** opened_doors, int max_dist);
static void open_new_door(t_map* m, t_list** opened_doors, int max_dist);

void door_routine(t_map* m)
{
    static t_list* opened_doors = 0;

    close_last_door(m, &opened_doors, 3);
    open_new_door(m, &opened_doors, 2);
    return;
}

static void close_last_door(t_map* m, t_list** opened_doors, int max_dist)
{
    t_list* head;
    t_list* curr;
    t_list* next;
    t_ivec2 door;
    int i;
    int x;
    int y;

    x = (int)man.player.pos.x;
    y = (int)man.player.pos.y;
    curr = *opened_doors;
    head = curr;
    while (curr)
    {
        next = curr->next;
        door = *((t_ivec2*)curr->data);
        i = -max_dist;
        while (i <= max_dist + 1)
        {
            if (i == max_dist + 1)
            {
                m->data[door.y * m->size.x + door.x] = 5;
                if (curr == head)
                    head = next;
                free(curr->data);
                free(curr);
                break;
            }
            if ((x == door.x + i && y == door.y) 
                || (x == door.x && y == door.y + i))
                break;
            ++i;
        }
        curr = next;
    }
    *opened_doors = head;
    return;
}

static void open_new_door(t_map* m, t_list** opened_doors, int max_dist)
{
    t_ivec2* coord;
    int i;
    int x;
    int y;

    x = (int)man.player.pos.x;
    y = (int)man.player.pos.y;
    i = -max_dist;
    while (i <= max_dist)
    {
        if (m->data[y * m->size.x + (x + i)] == 5)
        {
            coord = malloc(sizeof(t_ivec2));
            if (coord)
            {
                set_ivec2(coord, x + i, y);
                list_add_back(opened_doors, list_new(coord));
            }
            m->data[y * m->size.x + (x + i)] = 0;
            break;
        }
        else if (m->data[(y + i) * m->size.x + x] == 5)
        {
            coord = malloc(sizeof(t_ivec2));
            if (coord)
            {
                set_ivec2(coord, x, y + i);
                list_add_back(opened_doors, list_new(coord));
            }
            m->data[(y + i) * m->size.x + x] = 0;
            break;
        }
        ++i;
    }
    return;
}
