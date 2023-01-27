/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <drivers/mouse.h>

#include <quantum/init.h>

#include <sys/isr.h>
#include <sys/pio.h>

#include <drivers/vesa.h>

static int mouse_x = 0; 
static int mouse_y = 0;

static unsigned char mouse_cycle = 0; 
static char mouse_byte[3];  

void quantum_mouse_init()
{
    // Set default mouse cords
    mouse_x = get_screen_x() / 2;
    mouse_y = get_screen_y() / 2;

    // Enable mouse device
    mouse_wait(1);
    pio_outb(0x64, 0x20);
    
    // Enable interrupts
    mouse_wait(1);
    pio_outb(0x64, 0x20);
    mouse_wait(0);
    unsigned int status = pio_inb(0x60) | 2;
    mouse_wait(1);
    pio_outb(0x64, 0x60);
    mouse_wait(1);
    pio_outb(0x60, status);

    // Defulat settings
    mouse_write(0xF6);
    mouse_read();

    // Enable mouse
    mouse_write(0xF4);
    mouse_read();
    isr_register_interrupt_handler(IRQ_BASE + 12, mouse_handler);

    // Log that we successfully initialized ps2 mouse 
    quantum_info(__FILE__, 0, " Mouse  ", "Successfully initialized mouse drivers");
}

int get_mouse_x()
{
    return mouse_x;
}

int get_mouse_y()
{
    return mouse_y;
}

void mouse_wait(unsigned char type)
{
    unsigned int _time_out=100000;
    if (type == 0)
    {
        while (_time_out--)
        {
            if ((pio_inb(0x64) & 1) == 1)
            {
                return;
            }
        }
        return;
    } else {
        while (_time_out--)
        {
            if ((pio_inb(0x64) & 2) == 0)
            {
                return;
            }
        }
        return;
    }
} 

void mouse_write(unsigned char write)
{
    mouse_wait(1);
    pio_outb(0x64, 0xD4);
    mouse_wait(1);
    pio_outb(0x60, write);
}

unsigned char mouse_read()
{
    mouse_wait(0);
    return pio_inb(0x60);
}

void mouse_handler(__registers_t* regs)
{
    switch(mouse_cycle)
    {
        case 0:
            mouse_byte[0] = pio_inb(0x60);
            mouse_cycle++;
            break;
        case 1:
            mouse_byte[1] = pio_inb(0x60);
            mouse_cycle++;
            break;
        case 2:
            mouse_byte[2] = pio_inb(0x60);
            int dx, dy;
            dx = (mouse_byte[0] & (1 << 4)) ? (char)mouse_byte[1] : mouse_byte[1];
            dy = (mouse_byte[0] & (1 << 5)) ? (char)mouse_byte[2] : mouse_byte[2];
            mouse_x += dx;
            mouse_y -= dy;
            mouse_cycle = 0;
            break;
    }
}


