/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <userspace/window.h>

#include <drivers/vesa.h>

#include <sys/memory.h>

#include <core/string.h>

window_t* windows;
int windows_len = 0;

void init_windows()
{
    windows = (window_t*)kmalloc(sizeof(window_t) * 3);
}

window_t create_window(char* window_name, int size_x, int size_y, int pos_x, int pos_y)
{
    window_t window;

    int max_offset_framebuffer = (size_x * 4) + (size_y * (size_x * 4));
    window.framebuffer = kmalloc(sizeof(unsigned char) * max_offset_framebuffer);

    window.size_x = size_x;
    window.size_y = size_y;
    window.pos_x = pos_x;
    window.pos_y = pos_y;
    window.should_redraw = TRUE;

    strcpy(window.window_name, window_name);

    windows[windows_len] = window;
    windows_len++;
    return window;
}

void draw_windows()
{
    for (int i = 0; i < windows_len; i++)
    {
        window_t current_window = windows[i];
        debug_printf("%s\n", current_window.window_name);
        if (current_window.should_redraw == TRUE)
            render_window(current_window);
    }
}

void render_window(window_t window)
{
    vesa_draw_rect(window.pos_x, window.pos_y, window.size_x, window.size_y, 255, 255, 255);
}

void put_pixel_window(window_t window, int x, int y, int r, int g, int b)
{
    unsigned int offset = (x * 4) + (y * (x * 4));
    window.framebuffer[offset    ] = b;
    window.framebuffer[offset + 1] = g;
    window.framebuffer[offset + 2] = r;
}