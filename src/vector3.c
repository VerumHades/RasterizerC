#include "vector3.h"
#include <math.h>

Vector3 vector3_from_a_to_b(Vector3* a, Vector3* b){
    return (Vector3){ .x = b->x - a->x, .y = b->y - a->y, .z = b->z - a->z };
}

float_unit vector3_dot(Vector3* a, Vector3* b){
    return a->x*b->x + a->y*b->y + a->z*b->z;
}

Vector3 vector3_scale(Vector3* v, float s){
    return (Vector3){ v->x*s, v->y*s, v->z*s };
}

float_unit vector3_length(Vector3* v){
    return sqrtf(v->x*v->x + v->y*v->y + v->z*v->z);
}

Vector3 vector3_normalize(Vector3* v){
    float_unit len = vector3_length(v);
    if(len == 0.0f) return (Vector3){0,0,0};
    return vector3_scale(v, 1.0f / len);
}

Vector3 vector3_project_onto_normal(Vector3* a, Vector3* n){
    float_unit scale = vector3_dot(a, n);
    return vector3_scale(n, scale);
}

Vector3 vector3_add(Vector3* a, Vector3* b){
    return (Vector3){ a->x + b->x, a->y + b->y, a->z + b->z };
}

Vector3 vector3_cross(Vector3* a, Vector3* b){
    return (Vector3){
        .x = a->y * b->z - a->z * b->y,
        .y = a->z * b->x - a->x * b->z,
        .z = a->x * b->y - a->y * b->x
    };
}