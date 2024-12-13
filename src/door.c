#include "cimmerian.h"

static void close_last_door(t_map *m, t_list **opened_doors, int max_dist);
static void open_new_door(t_map *m, t_list **opened_doors, int max_dist);

void door_routine(t_map *m)
{
    static t_list *opened_doors = 0;

    close_last_door(m, &opened_doors, 3);
    open_new_door(m, &opened_doors, 2);
    return;
}

static void close_last_door(t_map *m, t_list **opened_doors, int max_dist)
{
    t_list *head;
    t_list *curr;
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
        door = *((t_ivec2 *)curr->data);
        i = -max_dist;
        while (i <= max_dist + 1)
        {
            if (i == max_dist + 1)
            {
                m->cells[door.y * m->size.x + door.x].is_obstacle = 1;
                if (curr == head)
                    head = curr->next;
                list_del_one(&curr, free);
                break;
            }
            if ((x == door.x + i && y == door.y) 
                || (x == door.x && y == door.y + i))
            {
                curr = curr->next;
                break;
            }
            ++i;
        }
    }
    *opened_doors = head;
    return;
}

static void open_new_door(t_map *m, t_list **opened_doors, int max_dist)
{
    t_ivec2 *coord;
    t_list *node;
    int i;
    int x;
    int y;

    x = (int)man.player.pos.x;
    y = (int)man.player.pos.y;
    i = -max_dist;
    while (i <= max_dist)
    {
        if (m->cells[y * m->size.x + (x + i)].is_door
            && m->cells[y * m->size.x + (x + i)].is_obstacle)
        {
            coord = malloc(sizeof(t_ivec2));
            if (coord)
            {
                set_ivec2(coord, x + i, y);
                node = list_new(coord);
                if (!node)
                    free(coord);
                else
                    list_add_back(opened_doors, node);
            }
            m->cells[y * m->size.x + (x + i)].is_obstacle = 0;
            break;
        }
        else if (m->cells[(y + i) * m->size.x + x].is_door
            && m->cells[(y + i) * m->size.x + x].is_obstacle)
        {
            coord = malloc(sizeof(t_ivec2));
            if (coord)
            {
                set_ivec2(coord, x, y + i);
                node = list_new(coord);
                if (!node)
                    free(coord);
                else
                    list_add_back(opened_doors, node);
            }
            m->cells[(y + i) * m->size.x + x].is_obstacle = 0;
            break;
        }
        ++i;
    }
    return;
}
