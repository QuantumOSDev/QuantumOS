/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <drivers/debug.h>

#include <core/string.h>
#include <core/stdarg.h>

#include <sys/memory.h>
#include <sys/pio.h>

void debug_putc(char c)
{
    pio_outb(DEBUG_PORT, c);
}

void debug_print(char* buf) 
{
    int i = 0;
    while (buf[i] != '\0')
        debug_putc(buf[i]);
}

void debug_printf_va(char* format, va_list arg)
{
    int format_int;
    double format_double;
    float format_float;
    long format_long;
    char format_char;
    char *format_str;
    char bufprint[1024];

    for (int i = 0; i < strlen(format); i++)
    {
        char c = format[i];

        if (c == '%')
        {
            i++;
            c = format[i];

            switch (c)
            {
            case '%':
                debug_putc('%');
                break;
            case 'd':
            case 'i':
            case 'D':
            case 'I':
            case 'o':
            case 'O':
            case 'x':
            case 'X':
            case 'h':
            case 'H':
                format_int = va_arg(arg, int);
                if (format_int < 0)
                {
                    debug_putc('-');
                    format_int *= -1;
                }

                if (c == 'd' || c == 'i' || c == 'D' || c == 'I') // Decimal
                    itoa(format_int, bufprint, 10);
                else if (c == 'o' || c == 'O') // Octals
                    itoa(format_int, bufprint, 8);
                else if (c == 'x' || c == 'X' || c == 'h' || c == 'H') // Hexadecimal
                    itoa(format_int, bufprint, 16);

                for (int i = 0; bufprint[i] != '\0'; i++)
                    debug_putc(bufprint[i]);
                kmemset((unsigned char *)bufprint, 0, 1024);
                break;
            case 'c':
            case 'C':
                format_char = va_arg(arg, int);
                debug_putc((char)format_char);
                break;
            case 's':
            case 'S':
                format_str = va_arg(arg, char *);
                for (int i2 = 0; i2 < strlen(format_str); i2++)
                    debug_putc(format_str[i2]);
                break;
            case 'l':
            case 'L':
                format_long = va_arg(arg, long long);
                ltoa(format_long, bufprint);
                for (int i = 0; bufprint[i] != '\0'; i++)
                    debug_putc(bufprint[i]);
                kmemset((unsigned char *)bufprint, 0, 1024);
            default:
                break;
            }
        }
        else
            debug_putc(c);
    }
}

void debug_printf(char* format, ...)
{
    va_list arg;
    va_start(arg, format);
    debug_printf_va(format, arg);
    va_end(arg);
}