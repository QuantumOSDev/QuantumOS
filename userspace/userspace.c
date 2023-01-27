/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <quantum/init.h>

#include <userspace/userspace.h>
#include <userspace/syscalls.h>
#include <userspace/usermode.h>
#include <userspace/window.h>
#include <userspace/elf.h>

#include <drivers/sound_blaster.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/debug.h>
#include <drivers/vesa.h>

#include <core/stdlib.h>
#include <core/string.h>
#include <core/print.h>
#include <core/stdarg.h>

#include <sys/process.h>
#include <sys/memory.h>
#include <sys/task.h>
#include <sys/cmos.h>
#include <sys/pit.h>

#include <fs/vfs.h>

static unsigned char mouse_bitmap[11 * 18] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 1
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 2
    1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, // 3
    1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, // 4
    1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, // 5
    1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, // 6
    1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, // 7
    1, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, // 8
    1, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0, // 9
    1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, // 10
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, // 11
    1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, // 12
    1, 2, 2, 2, 1, 2, 2, 1, 0, 0, 0, // 13
    1, 2, 2, 1, 0, 1, 2, 2, 1, 0, 0, // 14
    1, 2, 1, 0, 0, 1, 2, 2, 1, 0, 0, // 15
    1, 1, 0, 0, 0, 0, 1, 2, 2, 1, 0, // 16
    0, 0, 0, 0, 0, 0, 1, 2, 2, 1, 0, // 17
    0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, // 18
};

static short fps = 0;

static int mouse_x = 0;
static int mouse_y = 0;

void draw_mouse()
{
    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 11; x++)
        {
            if (mouse_bitmap[x + (y * 11)] == 0) 
                continue;
            else if (mouse_bitmap[x + (y * 11)] == 1)
                vesa_put_pixel_double_buffering(mouse_x + x, mouse_y + y, 0, 0, 0);
            else if (mouse_bitmap[x + (y * 11)] == 2)
                vesa_put_pixel_double_buffering(mouse_x + x, mouse_y + y, 255, 255, 255);
        }
    }
}

void render()
{
    draw_windows();
    draw_mouse();
    swap_buffers();
}

void inputs()
{
    mouse_x = get_mouse_x();
    mouse_y = get_mouse_y();
}

void userspace_initialize(void) 
{
    vesa_clear();
    set_default_pos();

    toggle_double_framebuffer();

    create_window("Terminal", 700, 450, 100, 100);

    unsigned long last_second = 0;
    unsigned long now = pit_get_ticks();

    short frames = 0;

    while (1)
    {
        now = pit_get_ticks();
        if (now - last_second > 1000)
        {
            fps = frames;
            frames = 0;
            last_second = now;
            quantum_info(__FILE__, 0, " GUI    ", "FPS: %d", fps);
        }

        frames++;
        clear_back_buffer();
        inputs();
        render();
    }
    
    for (;;)
    {}
}