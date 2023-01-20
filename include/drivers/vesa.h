#ifndef VESA_H
#define VESA_H

typedef enum __image_format_t {
    IMAGE_TGA,
    IMAGE_PNG,
    IMAGE_JPG,
} image_format_t;

void quantum_vesa_init(unsigned long mbinfo_ptr);

int get_screen_x();

int get_screen_y();

void vesa_clear();

void vesa_put_pixel(int x, int y, int r, int g, int b);

void vesa_draw_rect(int x, int y, int w, int h, int r, int g, int b);

void vesa_draw_circle(int center_x, int center_y, int radius, int r, int g, int b);

void vesa_draw_line(int start_x, int start_y, int end_x, int end_y, int r, int g, int b);

void vesa_draw_char(char c, int x, int y, int fg_r, int fg_g, int fg_b, int bg_r, int bg_g, int bg_b);

int vesa_draw_image(char* img_data, int img_data_size, image_format_t format);

#endif