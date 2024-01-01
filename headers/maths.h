#ifndef __MATHS_H__
#define __MATHS_H__

void compose_transform_matrix(float* matrix, 
    const float scale_x, const float scale_y, const float scale_z,
    const float rotate_x, const float rotate_y, const float rotate_z,
    const float translate_x, const float translate_y, const float translate_z);

void compose_perspective_projection_matrix(float* matrix, 
    const float fov, const float aspect_ratio, 
    const float near_clip, const float far_clip);

#endif

