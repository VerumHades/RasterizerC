#ifndef PIPELINE_H
#define PIPELINE_H

#include "math/vector2.h"
#include "screen.h"
#include "triangle.h"

typedef unsigned char byte;
typedef float pipeline_float;

#define MAX_VERTEX_SIZE 64

typedef struct VertexShaderEnvironment {
    Vector3* output;
} VertexShaderEnvironment;

typedef void(*VertexShader)(VertexShaderEnvironment* env, pipeline_float* data);

typedef struct FragmentShaderEnvironment {
    byte* pixel;
} FragmentShaderEnvironment;

typedef void(*FragmentShader)(FragmentShaderEnvironment* env, pipeline_float* data);

typedef struct VertexData {
    pipeline_float* data; // Raw packed vertex data

    size_t size; // Total size
    size_t vertex_size; // Single vertex size
} VertexData;

enum RenderResult {
    SUCCESS = 0,
    INVALID_VERTEX_DATA
};

int render_vertex_data(ScreenBuffer* buffer, ScreenBuffer* depth_buffer, VertexShader vertex_shader, FragmentShader fragment_shader, VertexData* vertex_data);

#endif // MATHPLUS_H
