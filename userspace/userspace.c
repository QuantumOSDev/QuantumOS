#include <userspace/userspace.h>
#include <userspace/syscalls.h>
#include <userspace/usermode.h>
#include <userspace/wallpaper.h>

#include <drivers/sound_blaster.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vesa.h>

#include <core/stdlib.h>
#include <core/string.h>
#include <core/print.h>
#include <core/stdarg.h>

#include <sys/process.h>
#include <sys/memory.h>
#include <sys/task.h>
#include <sys/pit.h>

#include <fs/vfs.h>

void task1()
{
    printf("Task 1\n");
}

void task2()
{
    printf("Task 2 that will be forked\n");
    fork();
}

void userspace_initialize(void) 
{
    vesa_clear();
    set_default_pos();

    process_spawn(task1, 0, 1000, 0, 1);
    process_spawn(task2, 0, 1000, 0, 1);

    for (;;) {}
}