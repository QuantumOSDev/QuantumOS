#include <drivers/vesa.h>

#include <multiboot.h>
#include <font.h>

unsigned long global_addr;

void quantum_vesa_init(unsigned long mbinfo_ptr) {
    global_addr = mbinfo_ptr;
}

void framebuffer_put_pixel(int x, int y, int r, int g, int b) {
    multiboot_info_t* mbinfo = (multiboot_info_t*)global_addr;
    unsigned char* framebuffer = (unsigned char*)mbinfo->framebuffer_addr;
    unsigned offset = x * 4 + y * mbinfo->framebuffer_pitch;

    framebuffer[offset    ] = r;
    framebuffer[offset + 1] = g;
    framebuffer[offset + 2] = b;
}

void draw_rect(int x, int y, int w, int h, int r, int g, int b) {
    for (int _x = 0; _x < w; _x++)
        for (int _y = 0; _y < h; _y++)
            framebuffer_put_pixel(x + _x, y + _y, r, g, b);
}

void draw_circle(int center_x, int center_y, int radius, int r, int g, int b) {
    int x = 0, y = radius;
    int d = 3 - 2 * radius;

    framebuffer_put_pixel(center_x + x, center_y + y, r, g, b);
    framebuffer_put_pixel(center_x - x, center_y + y, r, g, b);
    framebuffer_put_pixel(center_x + x, center_y - y, r, g, b);
    framebuffer_put_pixel(center_x - x, center_y - y, r, g, b);
    framebuffer_put_pixel(center_x + y, center_y + x, r, g, b);
    framebuffer_put_pixel(center_x - y, center_y + x, r, g, b);
    framebuffer_put_pixel(center_x + y, center_y - x, r, g, b);
    framebuffer_put_pixel(center_x - y, center_y - x, r, g, b);

    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else
            d = d + 4 * x + 6;

        framebuffer_put_pixel(center_x + x, center_y + y, r, g, b);
        framebuffer_put_pixel(center_x - x, center_y + y, r, g, b);
        framebuffer_put_pixel(center_x + x, center_y - y, r, g, b);
        framebuffer_put_pixel(center_x - x, center_y - y, r, g, b);
        framebuffer_put_pixel(center_x + y, center_y + x, r, g, b);
        framebuffer_put_pixel(center_x - y, center_y + x, r, g, b);
        framebuffer_put_pixel(center_x + y, center_y - x, r, g, b);
        framebuffer_put_pixel(center_x - y, center_y - x, r, g, b);
    }
}

void draw_line(int start_x, int start_y, int end_x, int end_y, int r, int g, int b) {
    int dx = end_x - start_x;
    int dy = end_y - start_y;
    int d = 2 * dy - dx;
    int y = start_y;

    for (int x = start_x; x <= end_x; x++) {
        framebuffer_put_pixel(x, y, r, g, b);
        if (d > 0) {
            y = y + 1;
            d = d - 2 * dx;
        }
        d = d + 2 * dy;
    }
}

void draw_character(char c, int x, int y, int fg_r, int fg_g, int fg_b, int bg_r, int bg_g, int bg_b) {
    unsigned char* font_char = &font_data[c * 16];

    for(int i = 0; i < 16; ++i){
        for(int j = 0; j < 8; ++j){
            if(font_char[i] & (1 << (8 - j))){
                framebuffer_put_pixel(x+j, y+i, fg_r, fg_g, fg_b);
            } else {
                framebuffer_put_pixel(x+j, y+i, bg_r, bg_g, bg_b);
            }
        }
    }
}