#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "units.h"
#include "vector2.h"
#include "vector3.h"
#include "projection.h"
#include "screen.h"
#include "triangle.h"
#include "mathplus.h"
#include "matrix4x4.h"

typedef unsigned char byte;

typedef struct ShaderEnvironment {
    Vector2* output;
} ShaderEnvironment;

typedef void(*Shader)(ShaderEnvironment* env, byte* data);

typedef struct VertexData {
    byte* data;
    size_t size;
    size_t vertex_size;
} VertexData;

enum RenderResult {
    SUCCESS = 0,
    INVALID_VERTEX_DATA
};

void printTriangle2(const Triangle2* tri) {
    if (!tri) return;
    for (int i = 0; i < 3; i++) {
        printf("Point %d: (%.3f, %.3f)\n", i,
               tri->points[i].x,
               tri->points[i].y);
    }
}


int render_triangle_data(ScreenBuffer* buffer, Shader shader, VertexData* vertex_data){
    size_t vertex_count = vertex_data->size / vertex_data->vertex_size;
    if(vertex_count % 3 != 0)
    return INVALID_VERTEX_DATA;
    
    for(size_t i = 0;i < vertex_data->size;i += vertex_data->vertex_size * 3){
    
        Triangle2 projected_triangle = {0};
        for(int j = 0;j < 3;j++){
            byte* vertex = &vertex_data->data[i + j * vertex_data->vertex_size];
            
            shader(&(ShaderEnvironment){
                .output = &projected_triangle.points[j]
            }, vertex);
            
            projected_triangle.points[j] = device_coordinates_to_screen(buffer->width, buffer->height, &projected_triangle.points[j]);
        }
        
        printTriangle2(&projected_triangle);
        const RGBPixel pixel = {.r = 100, .g = 150, .b = 10};
        fill_triangle(buffer, &projected_triangle, (unsigned char*)&pixel);
    }   
    
    return SUCCESS;
}

Camera* camera = NULL;
void default_shader(ShaderEnvironment* env, byte* vertex){
    Vector3* position = (Vector3*)vertex;
    
    Vector4 projected_position = matrix4x4_multiply_vector4(
        &camera->view_matrix, 
        &(Vector4){ .x = position->x, .y = position->y, .z = position->z, .w = 1.0f }
    );
    
    Vector2 final = project_point_in_camera_space_to_plane(&camera->plane, &projected_position);

    *env->output = final;
}

int main() {
    ScreenBuffer buffer = new_screen_buffer(1920, 1080, sizeof(RGBPixel));

    const FOV = 120;
    ProjectionPlane plane = create_projection_plane(
        1920,1080, 
        &(Vector3){0, 0, 0}, 
        &(Vector3){0, 0, 1}, 
        &(Vector3){0, 1, 0}, 
        degrees_to_radians(FOV / 2)
    );

    Camera _camera = create_camera(&plane);
    camera = &_camera;

    float_unit vertex_data[9] = {
        0.0f,  0.0f, 200.0f,  // bottom-left, in front of camera
        100.0f,100.0f, 150.0f,  // bottom-right
        0.0f,  100.0f, 200.0f,   // top-center
    };

    VertexData data = {
        .data = vertex_data,
        .size = 9 * sizeof(float_unit),
        .vertex_size = 3 * sizeof(float_unit)
    };


    render_triangle_data(&buffer, default_shader, &data);
    
    write_screen_to_ppm(&buffer, "image.ppm");

    // --- Cleanup ---
    free(buffer.data);

    return 0;
}
