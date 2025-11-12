#include "math/vector4.h"

Vector4 vector4_add(Vector4* a, Vector4* b){
    return (Vector4){
        .x = a->x + b->x,
        .y = a->y + b->y,
        .z = a->z + b->z,
        .w = a->w + b->w
    };
}

Vector4 vector4_scale(Vector4* v, float_unit s){
    return (Vector4){
        .x = v->x * s,
        .y = v->y * s,
        .z = v->z * s,
        .w = v->w * s
    };
}

float_unit vector4_dot(Vector4* a, Vector4* b){
    return a->x*b->x + a->y*b->y + a->z*b->z + a->w*b->w;
}
