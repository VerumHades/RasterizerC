#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pipeline.h"
#include "units.h"
#include "math/vector2.h"
#include "math/vector3.h"
#include "projection.h"
#include "screen.h"
#include "triangle.h"
#include "math/mathplus.h"
#include "math/matrix4x4.h"
#include <cpuid.h>

Matrix4x4* view_matrix = NULL;
ProjectionPlane* view_plane = NULL;

void default_shader(VertexShaderEnvironment* env, pipeline_float* vertex){
    Vector3* position = (Vector3*)vertex;
    
    Vector4 projected_position = matrix4x4_multiply_vector4(
        view_matrix, 
        &(Vector4){ .x = position->x, .y = position->y, .z = position->z, .w = 1.0f }
    );
    
    Vector2 final = project_point_in_camera_space_to_plane(view_plane, (Vector3*)&projected_position);

    *env->output = final;
}
void fragment_shader(FragmentShaderEnvironment* env, pipeline_float* vertex){

}


int main() {
    ScreenBuffer buffer = new_screen_buffer(1920, 1080, sizeof(RGBPixel));

    const float_unit FOV = 120;
    ProjectionPlane plane = create_projection_plane(
        1920,1080, 
        &(Vector3){0, 0, 0}, 
        &(Vector3){0, 0, 1}, 
        &(Vector3){0, 1, 0}, 
        degrees_to_radians(FOV / 2)
    );
    Matrix4x4 _view_matrix = build_view_matrix(&plane);
    view_plane = &plane;
    view_matrix = &_view_matrix;

    float_unit vertex_data[9] = {
        0.0f,  20.0f, 30.0f,   // top-center
        10.0f,10.0f, 20.0f,  // bottom-right
        0.0f,  0.0f, 20.0f,  // bottom-left, in front of camera
    };

    VertexData data = {
        .data = vertex_data,
        .size = 9,
        .vertex_size = 3
    };

    render_vertex_data(&buffer, default_shader, fragment_shader, &data);
    write_screen_to_bmp(&buffer, "image.bmp");

    // --- Cleanup ---
    free(buffer.data);

    return 0;
}
