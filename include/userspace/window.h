#ifndef WINDOW_H
#define WINDOW_H

#include <sys/process.h>
#include <core/stdlib.h>

typedef struct __window_t {
    char* window_name;

    int size_x;
    int size_y;

    int pos_x;
    int pos_y;

    unsigned char* framebuffer;
    BOOL should_redraw;
} window_t;

window_t create_window(char* window_name, int size_x, int size_y, int pos_x, int pos_y);

void draw_windows();

void render_window(window_t window);

void put_pixel_window(window_t window, int x, int y, int r, int g, int b);

#endif