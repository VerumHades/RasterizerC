#include "pipeline.h"
#include "projection.h"
#include "screen.h"
#include "triangle.h"
#include <string.h>
#include "math/mathplus.h"


int render_vertex_data(ScreenBuffer* buffer, VertexShader vertex_shader, FragmentShader fragment_shader, VertexData* vertex_data){
    size_t vertex_count = vertex_data->size / vertex_data->vertex_size;
    if(vertex_count % 3 != 0)
        return INVALID_VERTEX_DATA;
    
    for(size_t i = 0;i < vertex_data->size;i += vertex_data->vertex_size * 3){
    
        Triangle2 projected_triangle = {0};
        for(int j = 0;j < 3;j++){
            pipeline_float* vertex = &vertex_data->data[i + j * vertex_data->vertex_size];
            
            vertex_shader(&(VertexShaderEnvironment){
                .output = &projected_triangle.points[j]
            }, vertex);
            
            projected_triangle.points[j] = device_coordinates_to_screen(buffer->width, buffer->height, &projected_triangle.points[j]);
        }
        
        const RGBPixel pixel = {.r = 100, .g = 150, .b = 10};
        fill_triangle(buffer, &projected_triangle, (unsigned char*)&pixel);
    }   
    
    return SUCCESS;
}


int fill_triangle(ScreenBuffer* buffer, Triangle2* triangle, unsigned char* value){
    BoundingBox box = get_triangle_bounding_box(triangle);
    int x_start = clamp(box.min.x, 0, buffer->width);
    int x_end   = clamp(box.max.x, 0, buffer->width);
    int y_start = clamp(box.min.y, 0, buffer->height);
    int y_end   = clamp(box.max.y, 0, buffer->height);

    ComputedTriangleData data = compute_triangle_normals(triangle);

    for(int y = y_start; y < y_end; y++){
        for(int x = x_start; x < x_end; x++){
            Vector2 point = {.x=(float)x, .y=(float)y};
            if(point_in_triangle_precomputed(triangle, &data, &point)){
                int index = coordinates_to_buffer_index(buffer, x, y);
                memcpy(buffer->data + index * buffer->pixel_size, value, buffer->pixel_size);
            }
        }
    }
    return 0;
}
