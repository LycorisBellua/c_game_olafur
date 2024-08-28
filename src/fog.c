#include "cimmerian.h"

void update_dof(t_map* m, double increment)
{
    m->dof = f_clamp(m->dof + increment, 0, 30);
    m->fog_width = get_fog_width(m->dof);
    return;
}

double get_fog_width(double dof)
{
    if (dof < 2)
        return 1;
    else if (dof >= 2 && dof < 2.2)
        return 0.75;
    else if (dof >= 2.2 && dof < 2.4)
        return 0.70;
    else if (dof >= 2.4 && dof < 2.6)
        return 0.65;
    else if (dof >= 2.6 && dof < 2.8)
        return 0.6;
    else if (dof >= 2.8 && dof < 3)
        return 0.55;
    else if (dof >= 3 && dof < 3.25)
        return 0.5;
    else if (dof >= 3.25 && dof < 3.5)
        return 0.45;
    else if (dof >= 3.5 && dof < 3.75)
        return 0.4;
    else if (dof >= 3.75 && dof < 4)
        return 0.35;
    else if (dof >= 4 && dof < 5)
        return 0.3;
    else if (dof >= 5 && dof < 8)
        return 0.25;
    else if (dof >= 8 && dof < 8.5)
        return 0.2;
    else if (dof >= 8.5 && dof < 9)
        return 0.15;
    return 0.1;
}

void draw_floor(t_frame* f, double fog_width, t_color fog)
{
    int h_solid = f->size.y / 2 * fog_width;
    int h_gradient = f->size.y / 2 - h_solid;
    double factor;
    t_color bottom;
    t_vert v;

    bottom = get_color_rgba(42, 30, 30, 255);
    v.color = fog;
    v.coord.y = f->size.y / 2;
    while (v.coord.y < f->size.y / 2 + h_solid)
    {
        v.coord.x = 0;
        while (v.coord.x < f->size.x)
        {
            draw_point(f, v.color, v.coord.x, v.coord.y);
            ++v.coord.x;
        }
        ++v.coord.y;
    }
    while (v.coord.y < f->size.y)
    {
        v.coord.x = 0;
        factor = (double)(v.coord.y - f->size.y / 2 - h_solid) / h_gradient;
        while (v.coord.x < f->size.x)
        {
            v.color.r = (1 - factor) * fog.r + factor * bottom.r;
            v.color.g = (1 - factor) * fog.g + factor * bottom.g;
            v.color.b = (1 - factor) * fog.b + factor * bottom.b;
            v.color.a = 255;
            draw_point(f, v.color, v.coord.x, v.coord.y);
            ++v.coord.x;
        }
        ++v.coord.y;
    }
    return;
}

void draw_ceiling(t_frame* f, double fog_width, t_color fog)
{
    int h_solid = f->size.y / 2 * fog_width;
    int h_gradient = f->size.y / 2 - h_solid;
    double factor;
    t_color top;
    t_vert v;

    top = get_color_rgba(64, 0, 64, 255);
    v.coord.y = 0;
    while (v.coord.y < h_gradient)
    {
        v.coord.x = 0;
        factor = (double)v.coord.y / h_gradient;
        while (v.coord.x < f->size.x)
        {
            v.color.r = (1 - factor) * top.r + factor * fog.r;
            v.color.g = (1 - factor) * top.g + factor * fog.g;
            v.color.b = (1 - factor) * top.b + factor * fog.b;
            v.color.a = 255;
            draw_point(f, v.color, v.coord.x, v.coord.y);
            ++v.coord.x;
        }
        ++v.coord.y;
    }
    v.color = fog;
    while (v.coord.y < f->size.y / 2)
    {
        v.coord.x = 0;
        while (v.coord.x < f->size.x)
        {
            draw_point(f, v.color, v.coord.x, v.coord.y);
            ++v.coord.x;
        }
        ++v.coord.y;
    }
    return;
}

void apply_wall_fog(t_color* wall, t_color fog, double dist, double dof)
{
    double factor;

    factor = f_min(dist / dof, 1);
    wall->r = wall->r + factor * (fog.r - wall->r);
    wall->g = wall->g + factor * (fog.g - wall->g);
    wall->b = wall->b + factor * (fog.b - wall->b);
    return;
}
