#ifndef SCREEN_H
#define SCREEN_H

#include "units.h"
#include "math/vector2.h"

#include <stdio.h>

typedef struct ScreenBuffer {
    unsigned char* data;
    size_t pixel_size;
    int width, height;
} ScreenBuffer;

typedef struct RGBPixel {
    unsigned char r,g,b;
} RGBPixel;

ScreenBuffer new_screen_buffer(int width, int height, size_t pixel_size);
void fill_screen(ScreenBuffer* buffer, unsigned char* pixel);

// Only for RGBPixel data
void write_screen_to_ppm(ScreenBuffer* buffer, const char *filename);
void write_screen_to_bmp(ScreenBuffer* buffer, const char *filename);

int is_point_on_screen(ScreenBuffer* buffer, Vector2* point);
int coordinates_to_buffer_index(ScreenBuffer* buffer, int x, int y);

#endif // SCREEN_H
