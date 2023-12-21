#ifndef __INTERFACES_H__
#define __INTERFACES_H__

typedef void (*RenderFunction)(void);
typedef struct Interface Interface;

struct Interface
{
    RenderFunction render;
    Interface* previous;
    Interface* next;
};

extern Interface* active_interface;

void initialize_interfaces(void);
void set_active_interface(Interface* interface);

#endif

