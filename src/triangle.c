#include "triangle.h"
#include "math/mathplus.h"
#include "units.h"
#include "math/vector2.h"
#include <immintrin.h>

BoundingBox get_triangle_bounding_box(Triangle2* triangle){
    BoundingBox box = {0};
    box.min.x = box.min.y = 1e30f;  // start very large
    box.max.x = box.max.y = -1e30f; // start very small

    for(int i = 0; i < 3; i++){
        Vector2* point = &triangle->points[i];
        box.min.x = min(point->x, box.min.x);
        box.min.y = min(point->y, box.min.y);
        box.max.x = max(point->x, box.max.x);
        box.max.y = max(point->y, box.max.y);
    }

    box.width = box.max.x - box.min.x;
    box.height = box.max.y - box.min.y;
    return box;
}

ComputedTriangleData compute_triangle_normals(Triangle2* triangle){
    ComputedTriangleData data = {0};
    for(int i = 0; i < 3; i++){
        Vector2* a = &triangle->points[i];
        Vector2* b = &triangle->points[(i+1)%3];
        Vector2 edge = vector_from_a_to_b(a,b);
        data.normals_not_normalized[i] = rotated_90_degrees_clockwise(edge);
    }
    return data;
}

__m256 simd_4_pairs(float pair[2]){
    __m128 pairv = _mm_set_ps1(0.0f);
    pairv = _mm_loadl_pi(pairv, (__m64*)pair);
    pairv = _mm_movelh_ps(pairv, pairv);
    __m256 repeated = _mm256_broadcast_ps(&pairv);
    return repeated;
}

int point_in_triangle_precomputed(Triangle2* triangle, ComputedTriangleData* data, Vector2* point){
    __m256 point_4_pairs = simd_4_pairs((float*)point);

    const __m256i mask = _mm256_set_epi32(0,0,-1,-1,-1,-1,-1,-1);
    __m256 triangle_points = _mm256_maskload_ps((float*)triangle->points, mask);

    __m256 toward_point_vectors = _mm256_sub_ps(point_4_pairs, triangle_points);
    __m256 triangle_normals = _mm256_maskload_ps((float*)data->normals_not_normalized, mask);  

    __m256 dot_mult = _mm256_mul_ps(toward_point_vectors, triangle_normals);
    __m256 signed_areas_double = _mm256_hadd_ps(dot_mult,dot_mult);

    __m256 half = _mm256_set1_ps(0.5f);       // broadcast 0.5 to all lanes
    __m256 signed_areas_whole = _mm256_mul_ps( signed_areas_double , half);
    __m128 signed_areas_low = _mm256_castps256_ps128(signed_areas_whole);

    float first4[4];
    _mm_storeu_ps(first4, signed_areas_low);  

    return (first4[0] * first4[1] >= 0) &&  (first4[1] * first4[2] >= 0);
}