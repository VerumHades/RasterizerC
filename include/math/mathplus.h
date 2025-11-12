#ifndef MATHPLUS_H
#define MATHPLUS_H

#include "units.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static inline float_unit degrees_to_radians(float_unit degrees){
    return degrees * (M_PI / 180.0f);
}

static inline float_unit radians_to_degrees(float_unit radians){
    return radians * (180.0f / M_PI);
}


static inline float_unit min(float_unit a, float_unit b) {
    return a < b ? a : b;
}

static inline float_unit max(float_unit a, float_unit b) {
    return a > b ? a : b;
}

static inline float_unit clamp(float_unit value, float_unit low, float_unit high) {
    if(value < low) return low;
    if(value > high) return high;
    return value;
}

static inline int _floor(float_unit v){
    return (int)v; // simple truncation; can replace with floorf(v)
}

#endif // MATHPLUS_H
