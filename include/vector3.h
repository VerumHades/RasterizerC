#ifndef VECTOR3_H
#define VECTOR3_H

#include "units.h"

typedef struct Vector3 {
    float_unit x, y, z;
} Vector3;

Vector3 vector3_from_a_to_b(Vector3* a, Vector3* b);
float_unit vector3_dot(Vector3* a, Vector3* b);
Vector3 vector3_scale(Vector3* v, float s);
float_unit vector3_length(Vector3* v);
Vector3 vector3_normalize(Vector3* v);
Vector3 vector3_project_onto_normal(Vector3* a, Vector3* n);
Vector3 vector3_add(Vector3* a, Vector3* b);
Vector3 vector3_cross(Vector3* a, Vector3* b);

#endif // VECTOR3_H
