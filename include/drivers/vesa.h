#ifndef VESA_H
#define VESA_H

void quantum_vesa_init(unsigned long mbinfo_ptr);

void vesa_put_pixel(int x, int y, int r, int g, int b);

void vesa_draw_rect(int x, int y, int w, int h, int r, int g, int b);

void vesa_draw_circle(int center_x, int center_y, int radius, int r, int g, int b);

void vesa_draw_line(int start_x, int start_y, int end_x, int end_y, int r, int g, int b);

void vesa_draw_char(char c, int x, int y, int fg_r, int fg_g, int fg_b, int bg_r, int bg_g, int bg_b);

#endif