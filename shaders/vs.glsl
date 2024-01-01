#version 400

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_color;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec3 single_color;

out vec3 frag_color;

void main()
{
    mat4 mvp_matrix = projection_matrix * view_matrix * model_matrix;
    gl_Position = mvp_matrix * vec4(in_pos, 1.0);

    if (single_color == vec3(0.0))
        frag_color = in_color;
    else
        frag_color = single_color;
}

