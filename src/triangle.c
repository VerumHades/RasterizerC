#include "math/mathplus.h"
#include "triangle.h"
#include "units.h"
#include "math/vector2.h"
#include <immintrin.h>

BoundingBox get_triangle_bounding_box(Triangle3* triangle){
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

ComputedTriangleData compute_triangle_normals(Triangle3* triangle){
    ComputedTriangleData data = {0};
    for(int i = 0; i < 3; i++){
        Vector2* a = &triangle->points[i];
        Vector2* b = &triangle->points[(i+1)%3];
        Vector2 edge = vector_from_a_to_b(a,b);
        data.normals_not_normalized[i] = rotated_90_degrees_clockwise(edge);
    }
    return data;
}


int point_in_triangle_precomputed(Triangle3* triangle, ComputedTriangleData* data, Vector2* point, TriangleRenderResult* result){
    Vector3 a = triangle->points[0];
    Vector3 b = triangle->points[1];
    Vector3 c = triangle->points[2];
    Vector2 p = *point;

    float area_ABC = (b.x - a.x)*(c.y - a.y) - (c.x - a.x)*(b.y - a.y);
    float area_PBC = (b.x - p.x)*(c.y - p.y) - (c.x - p.x)*(b.y - p.y);
    float area_PCA = (c.x - p.x)*(a.y - p.y) - (a.x - p.x)*(c.y - p.y);
    float area_PAB = (a.x - p.x)*(b.y - p.y) - (b.x - p.x)*(a.y - p.y);

    result->uvs[0] = area_PBC / area_ABC;
    result->uvs[1] = area_PCA / area_ABC;
    result->uvs[2] = area_PAB / area_ABC;

    int inside = (area_PBC*area_PCA >= 0) && (area_PCA*area_PAB >= 0);
    result->clockwise = inside && area_PAB > 0;
    return inside; // clockwise check
}