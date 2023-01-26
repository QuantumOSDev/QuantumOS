/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <userspace/userspace.h>
#include <userspace/syscalls.h>
#include <userspace/usermode.h>
#include <userspace/elf.h>

#include <drivers/sound_blaster.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/debug.h>
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

void userspace_initialize(void) 
{
    vesa_clear();
    set_default_pos();
    
    for (;;)
{}
}