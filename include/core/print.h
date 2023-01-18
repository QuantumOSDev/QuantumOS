#ifndef PRINT_H
#define PRINT_H

#include <core/stdarg.h>

int set_print_x(int x);

int set_print_y(int y);

void quantum_print_init();

void set_default_pos();

void insert_tab();

void insert_newline();

void insert_backspace();

void putc(char c);

void printf(const char* format, ...);

void printf_va(char* format, va_list arg);

void print_set_color(int r, int g, int b);

void print_set_color_bg(int r, int g, int b);

#endif