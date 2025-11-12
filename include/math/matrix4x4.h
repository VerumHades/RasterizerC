#ifndef MATRIX_H
#define MATRIX_H

#include "units.h"
#include "math/vector4.h"

typedef struct Matrix4x4 {
    float_unit m[4][4];
} Matrix4x4;

// Multiply a Vector4 by a 4x4 matrix using your convention
Vector4 matrix4x4_multiply_vector4(Matrix4x4* mat, Vector4* vec);

#endif // MATRIX_H
