#include <quantum/kernel_menu.h>
#include <quantum/init.h>

#include <drivers/keyboard.h>
#include <drivers/vesa.h>

#include <core/print.h>

static int menu_position_selected = 0;
static unsigned char logo_bitmap[] = {
    // 1  2  3  4  5  6  7  8 |1  2  3  4  5  6  7  8 |1  2  3  4  5  6  7  8 |1  2  3  4  5  6  7  8 |
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void draw_logo()
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 32; x++)
        {
            if (logo_bitmap[(y * 32) + x])
        }
    }
}

void draw_menu()
{
    draw_logo();
}

void quantum_menu()
{
    quantum_info(2, " Menu   ", "Waiting for user decision");

    char key = 0;
    draw_menu();

    for (;;) 
    {
        key = keyboard_getchar();
        draw_menu();

        // Menu logic
    }   

}