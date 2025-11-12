#ifndef PIPELINE_H
#define PIPELINE_H

#include "math/vector2.h"
#include "screen.h"
#include "triangle.h"

typedef unsigned char byte;
typedef float pipeline_float;

typedef struct VertexShaderEnvironment {
    Vector2* output;
} VertexShaderEnvironment;

typedef void(*VertexShader)(VertexShaderEnvironment* env, pipeline_float* data);

typedef struct FragmentShaderEnvironment {
    byte* pixel;
} FragmentShaderEnvironment;

typedef void(*FragmentShader)(FragmentShaderEnvironment* env, pipeline_float* data);

typedef struct VertexData {
    pipeline_float* data;
    size_t size;
    size_t vertex_size;
} VertexData;

enum RenderResult {
    SUCCESS = 0,
    INVALID_VERTEX_DATA
};

int fill_triangle(ScreenBuffer* buffer, Triangle2* triangle, unsigned char* value);
int render_vertex_data(ScreenBuffer* buffer, VertexShader vertex_shader, FragmentShader fragment_shader, VertexData* vertex_data);

#endif // MATHPLUS_H
