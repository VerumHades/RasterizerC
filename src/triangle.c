#include "triangle.h"
#include "math/mathplus.h"
#include "units.h"
#include "math/vector2.h"
#include <immintrin.h>

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
    int sign = 0;

    for(int i = 0; i < 3; i++){
        Vector2* a = &triangle->points[i];
        Vector2 edge_to_point = vector_from_a_to_b(a, point);
        Vector2 normal = data->normals_not_normalized[i];

        float_unit signed_direction = dot(&normal, &edge_to_point);
        int new_sign = (signed_direction >= 0) ? 1 : -1;

        if(i != 0 && new_sign != sign) return 0;
        sign = new_sign; 
    }
    return 1;
}