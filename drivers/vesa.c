/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/multiboot.h>
#include <quantum/init.h>

#include <drivers/vesa.h>
#include <drivers/font.h>

#include <core/string.h>
#include <core/tga.h>

#include <sys/memory.h>

static BOOL is_double_framebuffer = FALSE;
static int screen_max_offset = 0;

multiboot_info_t* mbinfo;

static unsigned char* front_buffer;
static unsigned long global_addr;
static unsigned char back_buffer[3691520];

unsigned long get_global_mutliboot_addr()
{
    return global_addr;
}

void quantum_back_buffer_init()
{
    multiboot_info_t* mbinfo = (multiboot_info_t*)global_addr;
}

void toggle_double_framebuffer()
{
    if (is_double_framebuffer == TRUE)
        is_double_framebuffer = FALSE;
    else
        is_double_framebuffer = TRUE;
}

void quantum_vesa_init(unsigned long mbinfo_ptr)
{
    global_addr = mbinfo_ptr;
    mbinfo = (multiboot_info_t*)mbinfo_ptr;
    screen_max_offset = mbinfo->framebuffer_width * 4 + mbinfo->framebuffer_height * mbinfo->framebuffer_pitch;
    front_buffer = (unsigned char*)mbinfo->framebuffer_addr;
}

void vesa_put_pixel(int x, int y, int r, int g, int b) 
{
    multiboot_info_t* mbinfo = (multiboot_info_t*)global_addr;
    unsigned offset = x * 4 + y * mbinfo->framebuffer_pitch;

    front_buffer[offset    ] = b;
    front_buffer[offset + 1] = g;
    front_buffer[offset + 2] = r;
}

void vesa_put_pixel_double_buffering(int x, int y, int r, int g, int b) 
{
    multiboot_info_t* mbinfo = (multiboot_info_t*)global_addr;
    unsigned offset = x * 4 + y * mbinfo->framebuffer_pitch;

    back_buffer[offset    ] = b;
    back_buffer[offset + 1] = g;
    back_buffer[offset + 2] = r;
}

void swap_buffers()
{
    kmemcpy(front_buffer, back_buffer, screen_max_offset);
    kmemset(back_buffer, 0, screen_max_offset);
}

void clear_back_buffer()
{
    kmemset(back_buffer, 0, screen_max_offset);
}

int get_screen_x()
{
    multiboot_info_t* mbinfo = (multiboot_info_t*)global_addr;
    return mbinfo->framebuffer_width;
}

int get_screen_y() 
{
    multiboot_info_t* mbinfo = (multiboot_info_t*)global_addr;
    return mbinfo->framebuffer_height;
}

void vesa_draw_rect(int x, int y, int w, int h, int r, int g, int b) 
{
    if (is_double_framebuffer == FALSE)
        for (int _x = 0; _x < w; _x++)
            for (int _y = 0; _y < h; _y++)
                vesa_put_pixel(x + _x, y + _y, r, g, b);
    else
        for (int _x = 0; _x < w; _x++)
            for (int _y = 0; _y < h; _y++)
                vesa_put_pixel_double_buffering(x + _x, y + _y, r, g, b);
}

void vesa_clear() 
{
    multiboot_info_t* mbinfo = (multiboot_info_t*)global_addr;
    unsigned char* framebuffer = (unsigned char*)mbinfo->framebuffer_addr;
    
    kmemset(framebuffer, 0, mbinfo->framebuffer_width * 4 + mbinfo->framebuffer_height * mbinfo->framebuffer_pitch);
}

void vesa_draw_circle(int center_x, int center_y, int radius, int r, int g, int b) 
{
    if (is_double_framebuffer == FALSE)
    {
        int x = 0, y = radius;
        int d = 3 - 2 * radius;

        vesa_put_pixel(center_x + x, center_y + y, r, g, b);
        vesa_put_pixel(center_x - x, center_y + y, r, g, b);
        vesa_put_pixel(center_x + x, center_y - y, r, g, b);
        vesa_put_pixel(center_x - x, center_y - y, r, g, b);
        vesa_put_pixel(center_x + y, center_y + x, r, g, b);
        vesa_put_pixel(center_x - y, center_y + x, r, g, b);
        vesa_put_pixel(center_x + y, center_y - x, r, g, b);
        vesa_put_pixel(center_x - y, center_y - x, r, g, b);

        while (y >= x)
        {
            x++;
            if (d > 0)
            {
                y--;
                d = d + 4 * (x - y) + 10;
            } 
            else
                d = d + 4 * x + 6;

            vesa_put_pixel(center_x + x, center_y + y, r, g, b);
            vesa_put_pixel(center_x - x, center_y + y, r, g, b);
            vesa_put_pixel(center_x + x, center_y - y, r, g, b);
            vesa_put_pixel(center_x - x, center_y - y, r, g, b);
            vesa_put_pixel(center_x + y, center_y + x, r, g, b);
            vesa_put_pixel(center_x - y, center_y + x, r, g, b);
            vesa_put_pixel(center_x + y, center_y - x, r, g, b);
            vesa_put_pixel(center_x - y, center_y - x, r, g, b);
        }
    }
    else
    {
        int x = 0, y = radius;
        int d = 3 - 2 * radius;

        vesa_put_pixel_double_buffering(center_x + x, center_y + y, r, g, b);
        vesa_put_pixel_double_buffering(center_x - x, center_y + y, r, g, b);
        vesa_put_pixel_double_buffering(center_x + x, center_y - y, r, g, b);
        vesa_put_pixel_double_buffering(center_x - x, center_y - y, r, g, b);
        vesa_put_pixel_double_buffering(center_x + y, center_y + x, r, g, b);
        vesa_put_pixel_double_buffering(center_x - y, center_y + x, r, g, b);
        vesa_put_pixel_double_buffering(center_x + y, center_y - x, r, g, b);
        vesa_put_pixel_double_buffering(center_x - y, center_y - x, r, g, b);

        while (y >= x)
        {
            x++;
            if (d > 0)
            {
                y--;
                d = d + 4 * (x - y) + 10;
            } 
            else
                d = d + 4 * x + 6;

            vesa_put_pixel_double_buffering(center_x + x, center_y + y, r, g, b);
            vesa_put_pixel_double_buffering(center_x - x, center_y + y, r, g, b);
            vesa_put_pixel_double_buffering(center_x + x, center_y - y, r, g, b);
            vesa_put_pixel_double_buffering(center_x - x, center_y - y, r, g, b);
            vesa_put_pixel_double_buffering(center_x + y, center_y + x, r, g, b);
            vesa_put_pixel_double_buffering(center_x - y, center_y + x, r, g, b);
            vesa_put_pixel_double_buffering(center_x + y, center_y - x, r, g, b);
            vesa_put_pixel_double_buffering(center_x - y, center_y - x, r, g, b);
        }
    }
   
}

void vesa_draw_line(int start_x, int start_y, int end_x, int end_y, int r, int g, int b) 
{
    if (is_double_framebuffer == FALSE)
    {
        int dx = end_x - start_x;
        int dy = end_y - start_y;
        int d = 2 * dy - dx;
        int y = start_y;

        for (int x = start_x; x <= end_x; x++)
        {
            vesa_put_pixel(x, y, r, g, b);
            if (d > 0)
            {
                y = y + 1;
                d = d - 2 * dx;
            }
            d = d + 2 * dy;
        }
    }
    else
    {
        int dx = end_x - start_x;
        int dy = end_y - start_y;
        int d = 2 * dy - dx;
        int y = start_y;

        for (int x = start_x; x <= end_x; x++)
        {
            vesa_put_pixel_double_buffering(x, y, r, g, b);
            if (d > 0)
            {
                y = y + 1;
                d = d - 2 * dx;
            }
            d = d + 2 * dy;
        }
    }
}

void vesa_draw_char(char c, int x, int y, int fg_r, int fg_g, int fg_b, int bg_r, int bg_g, int bg_b) 
{
    
    unsigned char* font_char = &font_data[c * FONT_WIDTH];

    if (is_double_framebuffer == FALSE)
    {
        for (int i = 0; i < FONT_WIDTH; ++i)
            for (int j = 0; j < FONT_HEIGHT; ++j)
                if (font_char[i] & (1 << (FONT_HEIGHT - j)))
                    vesa_put_pixel(x+j, y+i, fg_r, fg_g, fg_b);
                else 
                    vesa_put_pixel(x+j, y+i, bg_r, bg_g, bg_b);
    }
    else
    {
        for (int i = 0; i < FONT_WIDTH; ++i)
            for (int j = 0; j < FONT_HEIGHT; ++j)
                if (font_char[i] & (1 << (FONT_HEIGHT - j)))
                    vesa_put_pixel_double_buffering(x+j, y+i, fg_r, fg_g, fg_b);
                // else 
                //     vesa_put_pixel_double_buffering(x+j, y+i, bg_r, bg_g, bg_b);
    }
    
}

int vesa_draw_image(char* img_data, int img_data_size, image_format_t format)
{
    // For now we don't support other image format than tga, why? because
    // tga is very easy to parse and we don't need anything more complex
    // for now
    if (format != IMAGE_TGA) return -1;
    
    if (img_data_size < 54) return -1;

    tga_structure_t* tga_structure = parse_tga(img_data, img_data_size);

    multiboot_info_t* mbinfo = (multiboot_info_t*)global_addr;
    unsigned char* framebuffer = (unsigned char*)mbinfo->framebuffer_addr;
    kmemcpy(framebuffer, tga_structure->pixel_data, img_data_size);
}