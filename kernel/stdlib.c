#include <core/stdlib.h>

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