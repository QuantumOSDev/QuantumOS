#include "vga.h"

#include "../pio/pio.h"

void __vga_set_cursor(int __offset)
{
    __offset /= 2;

    __pio_outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    __pio_outb(VGA_DATA_REGISTER, (unsigned char) (__offset >> 8));
    __pio_outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    __pio_outb(VGA_DATA_REGISTER, (unsigned char) (__offset & 0xff));
}

int __vga_get_cursor(void)
{
    __pio_outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);

    int __offset = __pio_inb(VGA_DATA_REGISTER) << 8;
    
    __pio_outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    
    __offset += __pio_inb(VGA_DATA_REGISTER);
    
    return __offset * 2;
}

int __vga_row_from_offset(int __offset) {
    return __offset / (2 * SCREEN_SIZE_COLS);
}

int __vga_line_from_offset(int __col, int __row) {
    return 2 * (__row * SCREEN_SIZE_COLS + __col);
}

void __vga_newline(void)
{
    VGA_CURSOR = __vga_line_from_offset(0, __vga_row_from_offset(VGA_CURSOR) + 1);
}

void __vga_tab(void)
{
	int offset = (SCREEN_SIZE_COLS * SCREEN_SIZE_BLEN) - (VGA_CURSOR % SCREEN_SIZE_COLS);

	if (offset < 4)
	{
		int remaining = 4 - offset;

		__vga_newline();

		VGA_CURSOR = (VGA_CURSOR) + remaining;

		return;
	}

	VGA_CURSOR = (VGA_CURSOR + 4);
}

void __vga_putchar(char __c, unsigned short __cl)
{
    if (__c == '\0')
    {
        return;
    }

    if (VGA_CURSOR >= SCREEN_SIZE)
    {
        __vga_clear();
    }

    if (__c == '\n')
    {
        __vga_newline();

        return;
    }

    if (__c == '\t')
    {
        __vga_tab();

        return;
    }

    VGA_POINTER[VGA_CURSOR]     = __c;
    VGA_POINTER[VGA_CURSOR + 1] = __cl;

    VGA_CURSOR = (VGA_CURSOR + 2);
}

void __vga_clear(void)
{
    VGA_CURSOR = 0;

    unsigned int __viter = VGA_CURSOR;

    while (__viter < SCREEN_SIZE)
    {
        __vga_putchar(' ', ATTR_BYTE_BLK_ON_BLK);

        __viter = VGA_CURSOR;
    }

    VGA_CURSOR = 0;
}

void __vga_backspace(void)
{
	VGA_CURSOR = (VGA_CURSOR - 2);

	VGA_POINTER[VGA_CURSOR] = ' ';
	VGA_POINTER[VGA_CURSOR + 1] = ATTR_BYTE_BLK_ON_BLK;
}