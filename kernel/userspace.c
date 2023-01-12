#include <drivers/keyboard.h>
#include <drivers/vesa.h>

#include <core/string.h>
#include <core/print.h>

#include <sys/userspace.h>

void userspace_initialize(void) {
    vesa_clear();
    set_default_pos();
    
    printf("Booting into userspace...\n");
    printf("TODO: start window server");
    for (;;) {}
}