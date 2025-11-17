#ifndef VECTOR3_H
#define VECTOR3_H

#include <stdint.h>

typedef float float_unit;

typedef struct {
    float_unit x;
    float_unit y;
    float_unit z;
} Vector3;

// Basic operations
Vector3 vector3_from_a_to_b(Vector3* a, Vector3* b);
float_unit vector3_dot(Vector3* a, Vector3* b);
Vector3 vector3_scale(Vector3* v, float s);
Vector3 vector3_add(Vector3* a, Vector3* b);
Vector3 vector3_sub(Vector3* a, Vector3* b);          // NEW
Vector3 vector3_mul_float(Vector3* v, float_unit s);  // NEW
Vector3 vector3_cross(Vector3* a, Vector3* b);
float_unit vector3_length(Vector3* v);
Vector3 vector3_normalize(Vector3* v);
Vector3 vector3_project_onto_normal(Vector3* a, Vector3* n);

#endif