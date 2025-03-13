#version 400 core

in vec2 frag_tex_coord;
uniform sampler2D frame_texture;

out vec4 frag_color;

void main()
{
    // GREYSCALE (GL_RED + GL_UNSIGNED_BYTE)
    //frag_color = vec4(texture(frame_texture, frag_tex_coord).r);

    vec3 colors = texture(frame_texture, frag_tex_coord).rgb;
    frag_color = vec4(colors, 1.0);
}
