#ifndef VECTOR4_H
#define VECTOR4_H

#include "units.h"

typedef struct Vector4 {
    float_unit x, y, z, w;
} Vector4;

// Vector4 operations following your naming convention
Vector4 vector4_add(Vector4* a, Vector4* b);
Vector4 vector4_scale(Vector4* v, float_unit s);
float_unit vector4_dot(Vector4* a, Vector4* b);

#endif // VECTOR4_H
