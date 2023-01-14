#ifndef DEBUG_H
#define DEBUG_H

#include <core/stdarg.h>

#define DEBUG_PORT 0x3F8

void debug_putc(char c);

void debug_print(char* buf);

void debug_printf_va(char* format, va_list arg);

void debug_printf(char* format, ...);

#endif