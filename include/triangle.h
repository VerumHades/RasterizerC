#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "units.h"
#include "vector2.h"
#include "vector3.h"
#include "screen.h"

typedef struct Triangle2 {
    Vector2 points[3];
} Triangle2;

typedef struct Triangle3 {
    Vector3 points[3];
} Triangle3;

typedef struct ComputedTriangleData {
    Vector2 normals_not_normalized[3];
} ComputedTriangleData;

typedef struct BoundingBox {
    Vector2 min;
    Vector2 max;
    float_unit width, height;
} BoundingBox;

BoundingBox get_triangle_bounding_box(Triangle2* triangle);
ComputedTriangleData compute_triangle_normals(Triangle2* triangle);
int point_in_triangle_precomputed(Triangle2* triangle, ComputedTriangleData* data, Vector2* point);
int fill_triangle(ScreenBuffer* buffer, Triangle2* triangle, unsigned char* value);

#endif // TRIANGLE_H
