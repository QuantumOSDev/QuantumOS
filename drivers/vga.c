/*
 * QuantumOS Copyright (c) 2022-2023
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <drivers/vga.h>

#include <sys/pio.h>

void vga_set_cursor(int __offset)
{
    __offset /= 2;

    pio_outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    pio_outb(VGA_DATA_REGISTER, (unsigned char) (__offset >> 8));
    pio_outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    pio_outb(VGA_DATA_REGISTER, (unsigned char) (__offset & 0xff));
}

int vga_get_cursor(void)
{
    pio_outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);

    int __offset = pio_inb(VGA_DATA_REGISTER) << 8;
    
    pio_outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    
    __offset += pio_inb(VGA_DATA_REGISTER);
    
    return __offset * 2;
}

int vga_row_from_offset(int __offset)
{
    return __offset / (2 * SCREEN_SIZE_COLS);
}

int vga_line_from_offset(int __col, int __row)
{
    return 2 * (__row * SCREEN_SIZE_COLS + __col);
}

void vga_newline(void)
{
    VGA_CURSOR = vga_line_from_offset(0, vga_row_from_offset(VGA_CURSOR) + 1);
}

void vga_tab(void)
{
	int offset = (SCREEN_SIZE_COLS * SCREEN_SIZE_BLEN) - (VGA_CURSOR % SCREEN_SIZE_COLS);

	if (offset < 4)
	{
		int remaining = 4 - offset;

		vga_newline();

		VGA_CURSOR = (VGA_CURSOR) + remaining;

		return;
	}

	VGA_CURSOR = (VGA_CURSOR + 4);
}

void vga_putchar(char __c, unsigned short __cl)
{
    if (__c == '\0')
    {
        return;
    }

    if (VGA_CURSOR >= SCREEN_SIZE)
    {
        vga_clear();
    }

    if (__c == '\n')
    {
        vga_newline();

        return;
    }

    if (__c == '\t')
    {
        vga_tab();

        return;
    }

    VGA_POINTER[VGA_CURSOR]     = __c;
    VGA_POINTER[VGA_CURSOR + 1] = __cl;

    VGA_CURSOR = (VGA_CURSOR + 2);
}

void vga_clear(void)
{
    VGA_CURSOR = 0;

    unsigned int __viter = VGA_CURSOR;

    while (__viter < SCREEN_SIZE)
    {
        vga_putchar(' ', ATTR_BYTE_BLK_ON_BLK);

        __viter = VGA_CURSOR;
    }

    VGA_CURSOR = 0;
}

void vga_backspace(void)
{
	VGA_CURSOR = (VGA_CURSOR - 2);

	VGA_POINTER[VGA_CURSOR] = ' ';
	VGA_POINTER[VGA_CURSOR + 1] = ATTR_BYTE_BLK_ON_BLK;
}
