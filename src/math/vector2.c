#include "math/vector2.h"
#include <math.h>

Vector2 vector_from_a_to_b(Vector2* a, Vector2* b){
    return (Vector2){ b->x - a->x, b->y - a->y };
}

Vector2 rotated_90_degrees_clockwise(Vector2 v){
    return (Vector2){ v.y, -v.x };
}

float_unit dot(Vector2* a, Vector2* b){
    return a->x * b->x + a->y * b->y;
}

Vector2 normalize(Vector2 v){
    float_unit len = sqrtf(v.x*v.x + v.y*v.y);
    if(len == 0) return (Vector2){0,0};
    return (Vector2){ v.x / len, v.y / len };
}
