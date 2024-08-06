/*
    Vsync caps the FPS to the monitor's refresh rate.
    1 to enable, 0 to disable.
*/
void enable_vsync(const int enable)
{
    glfwSwapInterval(enable != 0);
    return;
}
