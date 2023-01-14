#include <userspace/userspace.h>
#include <userspace/syscalls.h>

#include <drivers/sound_blaster.h>
#include <drivers/keyboard.h>
#include <drivers/vesa.h>

#include <core/stdlib.h>
#include <core/string.h>
#include <core/print.h>

#include <sys/process.h>
#include <sys/memory.h>

void userspace_initialize(void) 
{
    vesa_clear();
    set_default_pos();
    printf("Booting into userspace...\n");
}