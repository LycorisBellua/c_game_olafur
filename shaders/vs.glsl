#version 400

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_color;

uniform vec3 pos_offset;
uniform float yaw;
uniform float scale_factor;
uniform vec3 single_color;

out vec3 frag_color;

void main()
{
    // Transform the vertex position
    vec3 position = in_pos + pos_offset;
    mat3 rotation = mat3(
        vec3(cos(yaw), 0, sin(yaw)),
        vec3(0, 1, 0),
        vec3(-sin(yaw), 0, cos(yaw))
    );
    gl_Position = vec4(position * rotation * scale_factor, 1.0);

    // Output data to the fragment shader
    if (single_color == vec3(0.0))
        frag_color = in_color;
    else
        frag_color = single_color;
}

