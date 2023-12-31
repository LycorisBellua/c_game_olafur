#include "../headers/uniforms.h"
#include "../headers/shader_handling.h"
#include "../headers/maths.h"
#include "../headers/camera.h"

Uniform* uniforms[NBR_UNIFORMS] = {0};
static const float matrix_zero[16] = {0};

static void get_length_and_datatype(const ActivateUniformFunction activate, 
    int* length, GlslDatatype* type);
static void populate_data(void* data, const va_list args, const int length, 
    const GlslDatatype type);

void create_uniforms(void)
{
    float model_matrix[16] = {0};
    float view_matrix[16] = {0};
    float projection_matrix[16] = {0};

    /* Scale, Rotate, Translate */
    compose_transform_matrix(model_matrix, 
        1.0f, 1.0f, 1.0f, 
        0.0f, 45.0f, 0.0f, 
        0.0f, 0.0f, 0.0f);

    camera_transform[0] = 1.0f;
    camera_transform[1] = 1.0f;
    camera_transform[2] = 1.0f;

    camera_transform[3] = -20.0f;
    camera_transform[4] = 0.0f;
    camera_transform[5] = 0.0f;

    camera_transform[6] = 0.0f;
    camera_transform[7] = -0.5f;
    camera_transform[8] = 0.0f;

    compose_transform_matrix(view_matrix, 
        camera_transform[0], camera_transform[1], camera_transform[2], 
        camera_transform[3], camera_transform[4], camera_transform[5], 
        camera_transform[6], camera_transform[7], camera_transform[8]); 

    /* Example setup for perspective projection matrix */
    /*
       float fov = 60.0f;  // Field of view in degrees
       float aspectRatio = screenWidth / screenHeight;  // Adjust based on your screen dimensions
       float near_plane = 0.1f;
       float far_plane = 100.0f;
       compose_perspective_projection_matrix(projection_matrix, fov, aspect_ratio, near_plane, far_plane);
    compose_transform_matrix(projection_matrix, 1.0f, 1.0f, 1.0f, 45.0f, 30.0f, 60.0f, 2.0f, 3.0f, 1.0f);
    */

    UNIFORM_MODEL_MATRIX = create_uniform(shader_program_world->id, 
        "model_matrix", activate_uniform_mat4, model_matrix);
    UNIFORM_VIEW_MATRIX = create_uniform(shader_program_world->id, 
        "view_matrix", activate_uniform_mat4, view_matrix);
    UNIFORM_PROJECTION_MATRIX = create_uniform(shader_program_world->id, 
        "projection_matrix", activate_uniform_mat4, projection_matrix);
    UNIFORM_SINGLE_COLOR = create_uniform(shader_program_world->id, 
        "single_color", activate_uniform_vec3, 0.4f, 0.21f, 0.5f);
    uniforms[NBR_UNIFORMS - 1] = 0;
    return;
}

Uniform* create_uniform(const GLuint id_shader_program, const char* name, 
    const ActivateUniformFunction activate, ...)
{
    Uniform* u;
    va_list args;
    int length;
    GlslDatatype type;

    u = malloc(sizeof(Uniform));
    if (!u)
    {
        fprintf(stderr, "ERROR: Couldn't allocate memory for \"%s\" uniform.", 
            name);
        exit(EXIT_FAILURE);
    }

    /*
        A uniform can only have one shader location. If you want to use the 
        same uniform data in different shaders, different uniform names will 
        be necessary.
    */
    u->loc = glGetUniformLocation(id_shader_program, name);
    if (u->loc < 0)
    {
        fprintf(stderr, "ERROR: The \"%s\" uniform is either not found or "
            "unused in shader ID°%d.\n", name, id_shader_program);
        free(u);
        exit(EXIT_FAILURE);
    }

    u->activate = activate;
    u->data = 0;

    length = type = 0;
    get_length_and_datatype(activate, &length, &type);
    if (!length || !type)
    {
        fprintf(stderr, "ERROR: Unknown uniform type.\n");
        free(u);
        exit(EXIT_FAILURE);
    }

    if (type == FLOAT || type == MATRIX)
        u->data = malloc(length * sizeof(float));
    else if (type == INT)
        u->data = malloc(length * sizeof(int));
    else if (type == UINT)
        u->data = malloc(length * sizeof(unsigned int));

    if (!u->data)
    {
        fprintf(stderr, "ERROR: Couldn't allocate memory for \"%s\" uniform.\n", 
            name);
        free(u);
        exit(EXIT_FAILURE);
    }

    va_start(args, activate);
    populate_data(u->data, args, length, type);
    va_end(args);

    return u;
}

void free_uniforms(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(uniforms)/sizeof(Uniform*); ++i)
    {
        if (!uniforms[i])
            break;
        free_uniform(&uniforms[i]);
    }
    return;
}

void free_uniform(Uniform** u)
{
    /*
        Check whether the pointer is null. If it is, it means it doesn't 
        reference any struct, which therefore means that `data` is not a 
        pointer to a legal address.
    */
    if (!u)
        return;

    free((*u)->data);
    free(*u);

    /* Nullify the reference to prevent a double free */
    *u = 0;
    return;
}

/*
    GLSL basic datatypes
    - float floatNumber = 3.14;
    - int signedInt = -5;
    - uint unsignedInt = 10u;

    - [actually int] bool isTrue = true;
    - [doubles may not work]
*/

void activate_uniform_float(const Uniform* u, const int activate)
{
    if (activate)
        glUniform1f(u->loc, *(float*)u->data);
    else
        glUniform1f(u->loc, 0);
    return;
}

void activate_uniform_int(const Uniform* u, const int activate)
{
    if (activate)
        glUniform1i(u->loc, *(int*)u->data);
    else
        glUniform1i(u->loc, 0);
    return;
}

void activate_uniform_uint(const Uniform* u, const int activate)
{
    if (activate)
        glUniform1ui(u->loc, *(unsigned int*)u->data);
    else
        glUniform1ui(u->loc, 0);
    return;
}

/*
    GLSL vector datatypes
    - [float] vec2, vec3, vec4
    - [int] ivec2, ivec3, ivec4
    - [uint] uvec2, uvec3, uvec4

    - [bool] bvec2, bvec3, bvec4
    [Apparently GLSL treat them as actual booleans and not integers, 
    but there is no glUniform function to set booleans, so just use 
    integer vectors instead.]
*/

void activate_uniform_vec2(const Uniform* u, const int activate)
{
    if (activate)
        glUniform2fv(u->loc, 1, (float*)u->data);
    else
        glUniform2f(u->loc, 0, 0);
    return;
}

void activate_uniform_vec3(const Uniform* u, const int activate)
{
    if (activate)
        glUniform3fv(u->loc, 1, (float*)u->data);
    else
        glUniform3f(u->loc, 0, 0, 0);
    return;
}

void activate_uniform_vec4(const Uniform* u, const int activate)
{
    if (activate)
        glUniform4fv(u->loc, 1, (float*)u->data);
    else
        glUniform4f(u->loc, 0, 0, 0, 0);
    return;
}

void activate_uniform_ivec2(const Uniform* u, const int activate)
{
    if (activate)
        glUniform2iv(u->loc, 1, (int*)u->data);
    else
        glUniform2i(u->loc, 0, 0);
    return;
}

void activate_uniform_ivec3(const Uniform* u, const int activate)
{
    if (activate)
        glUniform3iv(u->loc, 1, (int*)u->data);
    else
        glUniform3i(u->loc, 0, 0, 0);
    return;
}

void activate_uniform_ivec4(const Uniform* u, const int activate)
{
    if (activate)
        glUniform4iv(u->loc, 1, (int*)u->data);
    else
        glUniform4i(u->loc, 0, 0, 0, 0);
    return;
}

void activate_uniform_uvec2(const Uniform* u, const int activate)
{
    if (activate)
        glUniform2uiv(u->loc, 1, (unsigned int*)u->data);
    else
        glUniform2ui(u->loc, 0, 0);
    return;
}

void activate_uniform_uvec3(const Uniform* u, const int activate)
{
    if (activate)
        glUniform3uiv(u->loc, 1, (unsigned int*)u->data);
    else
        glUniform3ui(u->loc, 0, 0, 0);
    return;
}

void activate_uniform_uvec4(const Uniform* u, const int activate)
{
    if (activate)
        glUniform4uiv(u->loc, 1, (unsigned int*)u->data);
    else
        glUniform4ui(u->loc, 0, 0, 0, 0);
    return;
}

/*
    GLSL matrix datatypes (= multidimensional array with float elements)
    - mat2: 2x2 array
    - mat3: 3x3 array
    - mat4: 4x4 array
*/

/*
    The `transpose` boolean parameter
    ---------------------------------------------------------------------------
    - Matrices are supplied in column-major order by default, where each 
    sequence of four contiguous elements represents a column of the matrix.

    matrix[0]   matrix[4]   matrix[8]    matrix[12]
    matrix[1]   matrix[5]   matrix[9]    matrix[13]
    matrix[2]   matrix[6]   matrix[10]   matrix[14]
    matrix[3]   matrix[7]   matrix[11]   matrix[15]

    - The other possibility is row-major order. In which case the transpose 
    argument has to be set to GL_TRUE so that OpenGL can interpret the data 
    properly.
    - Either way, matrices are stored in memory as 1D arrays, not 2D. For 
    example, a mat4 or a 4x4 matrix is not `matrix[4][4]` but `matrix[16]`.
*/

void activate_uniform_mat2(const Uniform* u, const int activate)
{
    if (activate)
        glUniformMatrix2fv(u->loc, 1, GL_FALSE, (float*)u->data);
    else
        glUniformMatrix2fv(u->loc, 1, GL_FALSE, matrix_zero);
    return;
}

void activate_uniform_mat3(const Uniform* u, const int activate)
{
    if (activate)
        glUniformMatrix3fv(u->loc, 1, GL_FALSE, (float*)u->data);
    else
        glUniformMatrix3fv(u->loc, 1, GL_FALSE, matrix_zero);
    return;
}

void activate_uniform_mat4(const Uniform* u, const int activate)
{
    if (activate)
        glUniformMatrix4fv(u->loc, 1, GL_FALSE, (float*)u->data);
    else
        glUniformMatrix4fv(u->loc, 1, GL_FALSE, matrix_zero);
    return;
}

/* ------------------------------------------------------------------------- */

static void get_length_and_datatype(const ActivateUniformFunction activate, 
    int* length, GlslDatatype* type)
{
    if (activate == activate_uniform_float)
    {
        *length = 1;
        *type = FLOAT;
    }
    else if (activate == activate_uniform_int)
    {
        *length = 1;
        *type = INT;
    }
    else if (activate == activate_uniform_uint)
    {
        *length = 1;
        *type = UINT;
    }
    else if (activate == activate_uniform_vec2)
    {
        *length = 2;
        *type = FLOAT;
    }
    else if (activate == activate_uniform_vec3)
    {
        *length = 3;
        *type = FLOAT;
    }
    else if (activate == activate_uniform_vec4)
    {
        *length = 4;
        *type = FLOAT;
    }
    else if (activate == activate_uniform_ivec2)
    {
        *length = 2;
        *type = INT;
    }
    else if (activate == activate_uniform_ivec3)
    {
        *length = 3;
        *type = INT;
    }
    else if (activate == activate_uniform_ivec4)
    {
        *length = 4;
        *type = INT;
    }
    else if (activate == activate_uniform_uvec2)
    {
        *length = 2;
        *type = UINT;
    }
    else if (activate == activate_uniform_uvec3)
    {
        *length = 3;
        *type = UINT;
    }
    else if (activate == activate_uniform_uvec4)
    {
        *length = 4;
        *type = UINT;
    }
    else if (activate == activate_uniform_mat2)
    {
        *length = 2*2;
        *type = MATRIX;
    }
    else if (activate == activate_uniform_mat3)
    {
        *length = 3*3;
        *type = MATRIX;
    }
    else if (activate == activate_uniform_mat4)
    {
        *length = 4*4;
        *type = MATRIX;
    }
    else
    {
        *length = 0;
        *type = 0;
    }
    return;
}

static void populate_data(void* data, const va_list args, const int length, 
    const GlslDatatype type)
{
    int i;
    float* matrix_ptr;

    if (type == FLOAT)
    {
        for (i = 0; i < length; ++i)
            *((float*)data + i) = va_arg(args, double);
    }
    else if (type == INT)
    {
        for (i = 0; i < length; ++i)
            *((int*)data + i) = va_arg(args, int);
    }
    else if (type == UINT)
    {
        for (i = 0; i < length; ++i)
            *((unsigned int*)data + i) = va_arg(args, unsigned int);
    }
    else if (type == MATRIX)
    {
        matrix_ptr = va_arg(args, float*);
        for (i = 0; i < length; ++i)
            *((float*)data + i) = matrix_ptr[i];
    }
    return;
}

