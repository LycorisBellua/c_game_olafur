#ifndef __INPUT_H__
#define __INPUT_H__

void physical_key_callback(GLFWwindow* window, const int key, 
    const int scancode, const int action, const int mods);
void char_key_callback(const GLFWwindow* window, const unsigned int codepoint);

#endif

