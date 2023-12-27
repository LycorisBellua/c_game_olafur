#version 400

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_color;

out vec3 frag_color;

void main()
{
    // No translation
    gl_Position = vec4(in_pos, 1.0);

    // Output data to the fragment shader
    frag_color = in_color;
}

