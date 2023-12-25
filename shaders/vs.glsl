#version 400

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_color;

uniform vec3 pos_offset;
uniform vec3 euler_angles;
uniform vec3 single_color;

out vec3 frag_color;

void main()
{
    // Transform the vertex position

    vec3 position = in_pos + pos_offset;

    mat3 rotation_yaw = mat3(
        vec3(cos(euler_angles.y), 0, -sin(euler_angles.y)),
        vec3(0, 1, 0),
        vec3(sin(euler_angles.y), 0, cos(euler_angles.y))
    );
    mat3 rotation_pitch = mat3(
        vec3(1, 0, 0),
        vec3(0, cos(euler_angles.x), sin(euler_angles.x)),
        vec3(0, -sin(euler_angles.x), cos(euler_angles.x))
    );
    mat3 rotation_roll = mat3(
        vec3(cos(euler_angles.z), sin(euler_angles.z), 0),
        vec3(-sin(euler_angles.z), cos(euler_angles.z), 0),
        vec3(0, 0, 1)
    );
    mat3 rotation = rotation_yaw * rotation_pitch * rotation_roll;

    gl_Position = vec4(in_pos * rotation + pos_offset, 1.0);




    // Output data to the fragment shader

    if (single_color == vec3(0.0))
        frag_color = in_color;
    else
        frag_color = single_color;
}

