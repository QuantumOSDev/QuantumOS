#include <print.h>
#include <string.h>

#include <drivers/vesa.h>
#include <sys/memory.h>
#include <stdarg.h>

int global_print_vec_x;
int global_print_vec_y;

int global_print_fg_r;
int global_print_fg_g;
int global_print_fg_b;

int global_print_bg_r;
int global_print_bg_g;
int global_print_bg_b;

void print_set_color(int r, int g, int b)
{
    global_print_fg_r = r;
    global_print_fg_g = g;
    global_print_fg_b = b;
}

void quantum_print_init()
{
    global_print_vec_x = 2;
    global_print_vec_y = 2;

    global_print_fg_r = 255;
    global_print_fg_g = 255;
    global_print_fg_b = 255;

    global_print_bg_r = 0;
    global_print_bg_g = 0;
    global_print_bg_b = 0;
    quantum_info(" Print  ", "Initializing print drivers");
}

void insert_tab()
{
    for (int i = 0; i < 4; i++)
        putc(' ');
}

void insert_newline()
{
    global_print_vec_x = 2;
    global_print_vec_y += 22;

    if (global_print_vec_y >= 720)
        global_print_vec_y = 0;
}

void putc(char c)
{
    if (c == '\n' || c == '\r')
        insert_newline();
    else if (c == '\t')
        insert_tab();
    else
    {
        vesa_draw_char(
            c,
            global_print_vec_x, global_print_vec_y,
            global_print_fg_r, global_print_fg_g, global_print_fg_b,
            global_print_bg_r, global_print_bg_g, global_print_bg_b);
        global_print_vec_x += 9;

        if (global_print_vec_x >= 1280)
            insert_newline();
    }
}

void printf(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);

    int format_int;
    double format_double;
    float format_float;
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
                putc('%');
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
                    putc('-');
                    format_int *= -1;
                }

                if (c == 'd' || c == 'i' || c == 'D' || c == 'I') // Decimal
                    itoa(format_int, bufprint, 10);
                else if (c == 'o' || c == 'O') // Octals
                    itoa(format_int, bufprint, 8);
                else if (c == 'x' || c == 'X' || c == 'h' || c == 'H') // Hexadecimal
                    itoa(format_int, bufprint, 16);

                for (int i = 0; bufprint[i] != '\0'; i++)
                    putc(bufprint[i]);
                kmemset((unsigned char *)bufprint, 0, 1024);
                break;
            case 'c':
            case 'C':
                format_char = va_arg(arg, int);
                putc((char)format_char);
                break;
            case 's':
            case 'S':
                format_str = va_arg(arg, char *);
                for (int i2 = 0; i2 < strlen(format_str); i2++)
                    putc(format_str[i2]);
                break;
            default:
                break;
            }
        }
        else
            putc(c);
    }

    va_end(arg);
}

void printf_va(char *format, va_list arg)
{
    int format_int;
    double format_double;
    float format_float;
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
                putc('%');
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
                    putc('-');
                    format_int *= -1;
                }

                if (c == 'd' || c == 'i' || c == 'D' || c == 'I') // Decimal
                    itoa(format_int, bufprint, 10);
                else if (c == 'o' || c == 'O') // Octals
                    itoa(format_int, bufprint, 8);
                else if (c == 'x' || c == 'X' || c == 'h' || c == 'H') // Hexadecimal
                    itoa(format_int, bufprint, 16);

                for (int i = 0; bufprint[i] != '\0'; i++)
                    putc(bufprint[i]);
                kmemset((unsigned char *)bufprint, 0, 1024);
                break;
            case 'c':
            case 'C':
                format_char = va_arg(arg, int);
                putc((char)format_char);
                break;
            case 's':
            case 'S':
                format_str = va_arg(arg, char *);
                for (int i2 = 0; i2 < strlen(format_str); i2++)
                    putc(format_str[i2]);
                break;
            default:
                break;
            }
        }
        else
            putc(c);
    }
}