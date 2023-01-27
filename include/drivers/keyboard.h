#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_COMMAND_PORT   0x64
#define KEYBOARD_STATUS_PORT    0x64
#define KEYBOARD_DATA_PORT      0x60

#define KEYBOARD_KEY_UP         0x80
#define KEYBOARD_KEY_DOWN       0x7F

#define KEYBOARD_READY          1

#define KEYBOARD_BACKSPACE_SC   0x0E
#define KEYBOARD_TAB_SC         0x0F
#define KEYBOARD_ENTER_SC       0x1C
#define KEYBOARD_CAPS_LOCK_SC   0x3A
#define KEYBOARD_LSHIFT_SC      0x2A
#define KEYBOARD_RSHIFT_SC      0x36

/* English keyboard map */
#define KEYBOARD_MAP_ENG {                                                      \
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',    \
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     \
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,          \
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',     \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0,    \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0,    \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0,       \
}

void keyboard_set_key_map(char* key_map);

char* keyboard_get_key_map();

char keyboard_getchar();

char keyboard_getchar_no_wait();

char *keyboard_getchar_until(char __c);

char keyboard_getsc();

void keyboard_enable();

#endif