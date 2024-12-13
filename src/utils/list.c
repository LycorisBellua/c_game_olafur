#include "cimmerian.h"

t_list *list_last(t_list *list)
{
    if (!list)
        return 0;
    while (list->next)
        list = list->next;
    return list;
}

t_list *list_new(void *data)
{
    t_list *new;

    new = malloc(sizeof(t_list));
    if (!new)
        return 0;
    new->prev = 0;
    new->next = 0;
    new->data = data;
    return new;
}

void list_add_front(t_list **list, t_list *new)
{
    if (!new)
        return;
    new->next = *list;
    *list = new;
    return;
}

void list_add_back(t_list **list, t_list *new)
{
    t_list *last;

    last = list_last(*list);
    if (!last)
        *list = new;
    else
    {
        last->next = new;
        new->prev = last;
    }
    return;
}

void list_del_one(t_list **list, void (*del)(void *))
{
    t_list *prev;
    t_list *next;

    if (!*list)
        return;
    prev = (*list)->prev;
    next = (*list)->next;
    if (del)
        del((*list)->data);
    free(*list);
    *list = next;
    if (prev)
        prev->next = next;
    if (next)
        next->prev = prev;
    return;
}
