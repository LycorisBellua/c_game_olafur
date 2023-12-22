#version 400

layout(location = 0) in vec3 in_pos;

uniform vec3 pos_offset;
uniform vec3 single_color;

out vec3 frag_color;

void main()
{
    // Transform the vertex position
    gl_Position = vec4(in_pos + pos_offset, 1.0);

    // Output data to the fragment shader
    if (single_color == vec3(0.0)
        single_color = vec3(1.0, 1.0, 1.0);
    frag_color = single_color;
}

