#version 400 core

in vec2 frag_tex_coord;
uniform sampler2D frame_texture;

out vec4 frag_color;

void main()
{
    frag_color = texture(frame_texture, frag_tex_coord);
}

