#include "screen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ScreenBuffer new_screen_buffer(int width, int height, size_t pixel_size){
    return (ScreenBuffer){
        .width = width,
        .height = height,
        .data = calloc(pixel_size, width*height),
        .pixel_size = pixel_size
    };
}

void write_screen_to_ppm(ScreenBuffer* buffer, const char *filename){
    FILE* f = fopen(filename, "w");
    fprintf(f, "P3\n%d %d\n255\n", buffer->width, buffer->height);

    for(int y=0;y<buffer->height;y++){
        for(int x=0;x<buffer->width;x++){
            int index = coordinates_to_buffer_index(buffer,x,y);
            RGBPixel* pixel = &((RGBPixel*)buffer->data)[index];
            fprintf(f,"%d %d %d ", (unsigned char)pixel->r, (unsigned char)pixel->g, (unsigned char)pixel->b);
        }
        fprintf(f,"\n");
    }

    fclose(f);
}

int is_point_on_screen(ScreenBuffer* buffer, Vector2* point){
    return point->x >= 0 && point->y >= 0 && point->x < buffer->width && point->y < buffer->height;
}

int coordinates_to_buffer_index(ScreenBuffer* buffer, int x, int y){
    return x + y*buffer->width;
}
