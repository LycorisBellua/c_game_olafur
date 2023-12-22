#version 400

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_color;

uniform vec3 pos_offset;
uniform vec3 single_color;

out vec3 frag_color;

void main()
{
    // Transform the vertex position
    gl_Position = vec4(in_pos + pos_offset, 1.0);

    // Output data to the fragment shader
    if (single_color == vec3(0.0))
        frag_color = in_color;
    else
        frag_color = single_color;
}

