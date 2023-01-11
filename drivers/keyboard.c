#include <drivers/keyboard.h>

#include <quantum/init.h>

#include <core/stdlib.h>
#include <core/string.h>
#include <core/print.h>

#include <sys/memory.h>

#include <sys/pio.h>
#include <sys/idt.h>
#include <sys/isr.h>

static BOOL __keyboard_shift_pressed = FALSE;
static BOOL __keyboard_caps_lock     = FALSE;

static char __keyboard_current_char  = 0;
static char __keyboard_current_sc    = 0;

char keyboard_alternate(char ch)
{
    switch(ch) {
        case '`': return '~';
        case '1': return '!';
        case '2': return '@';
        case '3': return '#';
        case '4': return '$';
        case '5': return '%';
        case '6': return '^';
        case '7': return '&';
        case '8': return '*';
        case '9': return '(';
        case '0': return ')';
        case '-': return '_';
        case '=': return '+';
        case '[': return '{';
        case ']': return '}';
        case '\\': return '|';
        case ';': return ':';
        case '\'': return '\"';
        case ',': return '<';
        case '.': return '>';
        case '/': return '?';

        default: return ch;
    }
}

char keyboard_getsc()
{
    int i;
    int scancode;

    for (i = 1000; i > 0; i++)
    {
        /* Check if scan code ready */

        if ((pio_inb(KEYBOARD_STATUS_PORT) & KEYBOARD_READY) == 0)
        {
            continue;
        }

        scancode = pio_inb(KEYBOARD_DATA_PORT);

        break;
    }

    /* If the scancode was received within the given time return it else return 0*/

    if (i > 0)
    {
        return scancode;
    }

    return 0;
}

char keyboard_getchar()
{
    char __result;

    while (__keyboard_current_char <= 0)
    {
        /* Wait */
    }

    __result = __keyboard_current_char;

    __keyboard_current_char = 0;

    return __result;
}

char *keyboard_getchar_until(char __c)
{
    char *__result = kcalloc(1, sizeof(char *));

    char __input = keyboard_getchar();

    while (__input != __c)
    {
        if (__input == '\b')
        {
            printf("\b");

            __result[strlen(__result) - 1] = '\0';

            __input = keyboard_getchar();

            continue;
        }

        printf("%c", __input);

        kstrcat(__result, (char[]) {__input, 0});

        __input = keyboard_getchar();
    }

    return __result;
}

static void keyboard_handler(registers_t *__regs)
{
    int scancode;

    __keyboard_current_char = 0;

    scancode = keyboard_getsc();

    if (scancode & KEYBOARD_KEY_UP)
    {
        /* Only valid for shifts */

        switch (scancode)
        {
            case KEYBOARD_LSHIFT_SC:
            {
                __keyboard_shift_pressed = FALSE;

                break;
            }

            case KEYBOARD_RSHIFT_SC:
            {
                __keyboard_shift_pressed = FALSE;

                break;
            }

            default:
            {
                break;
            }
        }
    }
    else
    {
        switch (scancode)
        {
            case KEYBOARD_CAPS_LOCK_SC:
            {
                if (__keyboard_caps_lock == FALSE)
                {
                    __keyboard_caps_lock = TRUE;
                }
                else
                {
                    __keyboard_caps_lock = FALSE;
                }

                break;
            }

            case KEYBOARD_ENTER_SC:
            {
                __keyboard_current_char = '\n';

                break;
            }

            case KEYBOARD_TAB_SC:
            {
                __keyboard_current_char = '\t';

                break;
            }

            case KEYBOARD_BACKSPACE_SC:
            {
                __keyboard_current_char = '\b';

                break;
            }

            case KEYBOARD_LSHIFT_SC:
            {
                __keyboard_shift_pressed = TRUE;

                break;
            }

            case KEYBOARD_RSHIFT_SC:
            {
                __keyboard_shift_pressed = TRUE;

                break;
            }

            default:
            {
                __keyboard_current_char = keyboard_map[scancode];

                if (__keyboard_caps_lock)
                {
                    if (__keyboard_shift_pressed)
                    {
                        __keyboard_current_char = keyboard_alternate(__keyboard_current_char);
                    }
                    else
                    {
                        if (((__keyboard_current_char >= 'A') && (__keyboard_current_char <= 'Z')) || ((__keyboard_current_char >= 'a') && (__keyboard_current_char <= 'z')))
                        {
                            __keyboard_current_char = __keyboard_current_char - 32;
                        }
                        else
                        {
                            __keyboard_current_char = keyboard_alternate(__keyboard_current_char);
                        }
                    }
                }
                else
                {
                    if (__keyboard_shift_pressed)
                    {
                        if (((__keyboard_current_char >= 'A') && (__keyboard_current_char <= 'Z')) || ((__keyboard_current_char >= 'a') && (__keyboard_current_char <= 'z')))
                        {
                            __keyboard_current_char = __keyboard_current_char - 32;
                        }
                        else
                        {
                            __keyboard_current_char = keyboard_alternate(__keyboard_current_char);
                        }
                    }
                    else
                    {
                        __keyboard_current_char = keyboard_map[scancode];
                    }
                }

                break;
            }
        }
    }
}

void keyboard_enable()
{
    quantum_info(0, "Keyboard", "Initializing keyboard drivers");
    isr_register_interrupt_handler(IRQ_BASE + 1, keyboard_handler);
}
