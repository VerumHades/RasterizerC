/*void draw_pixel_terminal(unsigned char r, unsigned char g, unsigned char b) {
    printf("\x1b[38;2;%d;%d;%dm██", r, g, b);
}

void draw_screen_to_terminal(ScreenBuffer* buffer) {
    const int w = buffer->width;
    const int h = buffer->height;

    printf("\x1b[H");

    // Worst-case: 24 bytes per pixel
    size_t max = (size_t)w * h * 24 + h + 8;
    char *out = malloc(max);
    char *p = out;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {

            int index = coordinates_to_buffer_index(buffer, x, y);
            RGBPixel px = ((RGBPixel*)buffer->data)[index];

            // Truecolor ANSI (foreground pixel)
            p += sprintf(p, "\x1b[38;2;%d;%d;%dm██", px.r, px.g, px.b);
        }
        *p++ = '\n';
    }

    // Reset terminal color
    p += sprintf(p, "\x1b[0m");

    fwrite(out, 1, (size_t)(p - out), stdout);
    fflush(stdout);

    free(out);
}*/

    /*
    const RGBPixel black = {.r=0,.g=0,.b=0};
    int step = 1;
    for(;;){
        if(plane.origin.x > 20) step = -1;
        if(plane.origin.x < -20) step = 1;

        plane.origin.x += step;
        _view_matrix = build_view_matrix(&plane);

        fill_screen(&buffer, (byte*)&black);
        draw_screen_to_terminal(&buffer);
    }*/
