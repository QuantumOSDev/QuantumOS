/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 *  - CodeSploit <samuelthart@pm.me>
 */

#include <core/stdlib.h>
#include <core/string.h>
#include <core/stdarg.h>

#include <sys/memory.h>

int kstrlen(const char *__data)
{
    unsigned int __length = 0;

    while (__data[__length])
    {
        __length++;
    }

    return __length;
}

void kstrcat(char *__dest, const char *__src)
{
    char *__end_ptr = (char *) __dest + kstrlen(__dest);

    kmemcpy((void *) __end_ptr, (void *) __src, kstrlen(__src));

    __end_ptr = (__end_ptr + kstrlen(__src));

    *__end_ptr = '\0';
}

int isalpha(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int isdigit(char c)
{
    return (c >= '0' && c <= '9');
}

int isspace(char c)
{
    return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

void append_str(char* buf, char c)
{
    int len = strlen(buf);
    buf[len]     = c;
    buf[len + 1] = '\0';
}

void sprintf(char* buf, char* format, ...) 
{
    va_list arg;
    va_start(arg, format);

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
                append_str(buf, '%');
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
                    append_str(buf, '-');
                    format_int *= -1;
                }

                if (c == 'd' || c == 'i' || c == 'D' || c == 'I') // Decimal
                    itoa(format_int, bufprint, 10);
                else if (c == 'o' || c == 'O') // Octals
                    itoa(format_int, bufprint, 8);
                else if (c == 'x' || c == 'X' || c == 'h' || c == 'H') // Hexadecimal
                    itoa(format_int, bufprint, 16);

                for (int i = 0; bufprint[i] != '\0'; i++)
                    append_str(buf, bufprint[i]);
                kmemset((unsigned char *)bufprint, 0, 1024);
                break;
            case 'c':
            case 'C':
                format_char = va_arg(arg, int);
                append_str(buf, (char)format_char);
                break;
            case 's':
            case 'S':
                format_str = va_arg(arg, char *);
                for (int i2 = 0; i2 < strlen(format_str); i2++)
                    append_str(buf, format_str[i2]);
                break;
            case 'l':
            case 'L':
                format_long = va_arg(arg, long long);
                ltoa(format_long, bufprint);
                for (int i = 0; bufprint[i] != '\0'; i++)
                    append_str(buf, bufprint[i]);
                kmemset((unsigned char *)bufprint, 0, 1024);
            default:
                break;
            }
        }
        else
            append_str(buf, c);
    }

    va_end(arg);
}

void append_str_sn(char* buf, int buf_size, char c)
{
    int len = strlen(buf);
    if (buf_size <= len) return;

    buf[len]     = c;
    buf[len + 1] = '\0';
}

void snprintf(char* buf, int buf_size, char* format, ...) 
{
    va_list arg;
    va_start(arg, format);

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
                append_str(buf, '%');
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
                    append_str(buf, '-');
                    format_int *= -1;
                }

                if (c == 'd' || c == 'i' || c == 'D' || c == 'I') // Decimal
                    itoa(format_int, bufprint, 10);
                else if (c == 'o' || c == 'O') // Octals
                    itoa(format_int, bufprint, 8);
                else if (c == 'x' || c == 'X' || c == 'h' || c == 'H') // Hexadecimal
                    itoa(format_int, bufprint, 16);

                for (int i = 0; bufprint[i] != '\0'; i++)
                    append_str(buf, bufprint[i]);
                kmemset((unsigned char *)bufprint, 0, 1024);
                break;
            case 'c':
            case 'C':
                format_char = va_arg(arg, int);
                append_str(buf, (char)format_char);
                break;
            case 's':
            case 'S':
                format_str = va_arg(arg, char *);
                for (int i2 = 0; i2 < strlen(format_str); i2++)
                    append_str(buf, format_str[i2]);
                break;
            case 'l':
            case 'L':
                format_long = va_arg(arg, long long);
                ltoa(format_long, bufprint);
                for (int i = 0; bufprint[i] != '\0'; i++)
                    append_str(buf, bufprint[i]);
                kmemset((unsigned char *)bufprint, 0, 1024);
            default:
                break;
            }
        }
        else
            append_str(buf, c);
    }

    va_end(arg);
}
