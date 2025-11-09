#include "matrix4x4.h"

Vector4 matrix4x4_multiply_vector4(Matrix4x4* mat, Vector4* vec){
    Vector4 result;
    result.x = mat->m[0][0]*vec->x + mat->m[0][1]*vec->y + mat->m[0][2]*vec->z + mat->m[0][3]*vec->w;
    result.y = mat->m[1][0]*vec->x + mat->m[1][1]*vec->y + mat->m[1][2]*vec->z + mat->m[1][3]*vec->w;
    result.z = mat->m[2][0]*vec->x + mat->m[2][1]*vec->y + mat->m[2][2]*vec->z + mat->m[2][3]*vec->w;
    result.w = mat->m[3][0]*vec->x + mat->m[3][1]*vec->y + mat->m[3][2]*vec->z + mat->m[3][3]*vec->w;
    return result;
}
