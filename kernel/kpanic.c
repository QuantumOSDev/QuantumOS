#include <core/print.h>

void kpanic(const char *__message)
{
    printf("KERNEL PANIC: [%s]\nHALTING THE SYSTEM!", __message);

    asm volatile ("hlt");
}
