#ifndef VECTOR2_H
#define VECTOR2_H

#include "units.h"

typedef struct Vector2 {
    float_unit x, y;
} Vector2;

Vector2 vector_from_a_to_b(Vector2* a, Vector2* b);
Vector2 rotated_90_degrees_clockwise(Vector2 v);
Vector2 normalize(Vector2 v);
float_unit dot(Vector2* a, Vector2* b);

#endif // VECTOR2_H
