#include "screen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void write_screen_to_bmp(ScreenBuffer* buffer, const char *filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;

    int width = buffer->width;
    int height = buffer->height;
    int row_padded = (width * 3 + 3) & (~3); // rows must be multiple of 4 bytes
    int filesize = 54 + row_padded * height; // 54 bytes header

    unsigned char bmpfileheader[14] = {
        'B','M',            // Signature
        0,0,0,0,            // File size
        0,0,                // Reserved1
        0,0,                // Reserved2
        54,0,0,0            // Offset to pixel data
    };

    unsigned char bmpinfoheader[40] = {
        40,0,0,0,           // Header size
        0,0,0,0,            // Width
        0,0,0,0,            // Height
        1,0,                // Planes
        24,0,               // Bits per pixel
        0,0,0,0,            // Compression
        0,0,0,0,            // Image size
        0,0,0,0,            // X pixels per meter
        0,0,0,0,            // Y pixels per meter
        0,0,0,0,            // Total colors
        0,0,0,0             // Important colors
    };

    // Fill in file size
    bmpfileheader[2] = (unsigned char)(filesize      );
    bmpfileheader[3] = (unsigned char)(filesize >>  8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);

    // Fill in width
    bmpinfoheader[4] = (unsigned char)(width      );
    bmpinfoheader[5] = (unsigned char)(width >>  8);
    bmpinfoheader[6] = (unsigned char)(width >> 16);
    bmpinfoheader[7] = (unsigned char)(width >> 24);

    // Fill in height
    bmpinfoheader[8] = (unsigned char)(height      );
    bmpinfoheader[9] = (unsigned char)(height >>  8);
    bmpinfoheader[10] = (unsigned char)(height >> 16);
    bmpinfoheader[11] = (unsigned char)(height >> 24);

    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);

    unsigned char* row = (unsigned char*)malloc(row_padded);
    for (int y = height - 1; y >= 0; y--) { // BMP is bottom-up
        for (int x = 0; x < width; x++) {
            int idx = coordinates_to_buffer_index(buffer, x, y);
            RGBPixel* pixel = &((RGBPixel*)buffer->data)[idx];
            row[x * 3 + 0] = pixel->b;
            row[x * 3 + 1] = pixel->g;
            row[x * 3 + 2] = pixel->r;
        }
        for (int i = width * 3; i < row_padded; i++) row[i] = 0;
        fwrite(row, 1, row_padded, f);
    }

    free(row);
    fclose(f);
}

ScreenBuffer new_screen_buffer(int width, int height, size_t pixel_size){
    return (ScreenBuffer){
        .width = width,
        .height = height,
        .data = calloc(pixel_size, width*height),
        .pixel_size = pixel_size
    };
}

void fill_screen(ScreenBuffer* buffer, unsigned char* pixel) {
    int total_pixels = buffer->width * buffer->height;
    
    for (int i = 0; i < total_pixels; i++) {
        memcpy(buffer->data + i * buffer->pixel_size, pixel, buffer->pixel_size);
    }
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
