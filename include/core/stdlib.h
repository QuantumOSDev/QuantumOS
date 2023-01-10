#ifndef STDLIB_H
#define STDLIB_H

typedef enum
{
    FALSE,
    TRUE,
} BOOL;

#define NULL (void *) 0

int kstrlen(const char *__data);

void kstrcat(char *__dest, const char *__src);

#endif