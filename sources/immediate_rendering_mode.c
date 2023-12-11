#include <GLFW/glfw3.h>
#include "../headers/immediate_rendering_mode.h"

/*
    Immediate rendering mode
    ---------------------------------------------------------------------------
    It was my understanding that we can make do without shaders, but turns out 
    that doing so has been deprecated since OpenGL 2.0 (2004). For learning 
    purposes, here are IRM functions, aka the deprecated way of doing things.

    There are indeed two different pipelines with their associated rendering 
    mode:
    - the fixed-function pipeline and its immediate rendering mode,
    - the programmable pipeline and its retained rendering mode.

    Shaders are mini programs, hence why it's called "programmable". The 
    difference between the two modes is that, in immediate mode, each 
    instruction is sent to the GPU as soon as it's ready, whereas in retained 
    mode the instructions are put together in a batch and sent to the GPU in 
    one go. Well, there can be the need for several batches, depending on the 
    complexity of the scene, but as one batch = one draw call, it's still way 
    more efficient than having a draw call per instruction.

    Also, of course, no matter which mode is used, the frame is still not 
    drawn before our very eyes. Even in immediate mode it's rendered in the 
    back buffer, and then we decide when to swap (cf. buffer swapping). 
*/

/*
    XYZ coordinates
    ---------------------------------------------------------------------------
    In OpenGL, the center of the viewport is the origin of the world (0,0,0). 
    X is the horizontal axis, Y the vertical axis, and Z the forward axis 
    (= depth). In 2D projection, the Z axis can just be 0. Another thing to 
    note is that coordinates are normalized, which means a range of [-1, 1].
*/

void irm_triangle_white(void)
{
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(0.0, 0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glVertex3f(-0.5, -0.5, 0.0);
    glEnd();
    return;
}

void irm_triangle_rgb(void)
{
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-0.5, -0.5, 0.0);
    glEnd();
    return;
}

void irm_rectangle_bicolor(void)
{
    /* Red */
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-0.5, 0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glVertex3f(-0.5, -0.5, 0.0);
    glEnd();

    /* Green */
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-0.5, 0.5, 0.0);
    glVertex3f(0.5, 0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glEnd();
    return;
}

void irm_viewport_white(void)
{
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-1, 1, 0.0);
    glVertex3f(1, 1, 0.0);
    glVertex3f(1, -1, 0.0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-1, 1, 0.0);
    glVertex3f(1, -1, 0.0);
    glVertex3f(-1, -1, 0.0);
    glEnd();
    return;
}

