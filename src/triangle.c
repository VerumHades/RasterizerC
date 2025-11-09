#include "triangle.h"
#include "mathplus.h"
#include <string.h>

BoundingBox get_triangle_bounding_box(Triangle2* triangle){
    BoundingBox box = {0};
    box.min.x = box.min.y = 1e30f;  // start very large
    box.max.x = box.max.y = -1e30f; // start very small

    for(int i = 0; i < 3; i++){
        Vector2* point = &triangle->points[i];
        box.min.x = min(point->x, box.min.x);
        box.min.y = min(point->y, box.min.y);
        box.max.x = max(point->x, box.max.x);
        box.max.y = max(point->y, box.max.y);
    }

    box.width = box.max.x - box.min.x;
    box.height = box.max.y - box.min.y;
    return box;
}

ComputedTriangleData compute_triangle_normals(Triangle2* triangle){
    ComputedTriangleData data = {0};
    for(int i = 0; i < 3; i++){
        Vector2* a = &triangle->points[i];
        Vector2* b = &triangle->points[(i+1)%3];
        Vector2 edge = vector_from_a_to_b(a,b);
        data.normals_not_normalized[i] = rotated_90_degrees_clockwise(edge);
    }
    return data;
}

int point_in_triangle_precomputed(Triangle2* triangle, ComputedTriangleData* data, Vector2* point){
    for(int i = 0; i < 3; i++){
        Vector2* a = &triangle->points[i];
        Vector2 edge_to_point = vector_from_a_to_b(a, point);
        Vector2 normal = data->normals_not_normalized[i];

        float_unit delta = dot(&normal, &edge_to_point);
        
        if(dot(&normal, &edge_to_point) < 0) return 0;
    }
    return 1;
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
