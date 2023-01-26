#ifndef KPANIC_H
#define KPANIC_H

typedef struct stack_frame_t {
    struct stack_frame_t* ebp;
    unsigned int eip;
} stack_frame_t;

void kpanic(const char *__message);

#endif
