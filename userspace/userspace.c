#include <userspace/userspace.h>
#include <userspace/syscalls.h>

#include <drivers/keyboard.h>
#include <drivers/vesa.h>

#include <core/stdlib.h>
#include <core/string.h>
#include <core/print.h>

#include <sys/process.h>

int process1() 
{
    printf("Hello, World!\n");
    return 69;
}

void userspace_initialize(void) 
{
    vesa_clear();
    set_default_pos();
    // switch_to_user_mode);
    printf("Booting into userspace...\n");
    
    process_t* process = init_process(&process1);
    start_process(process);
}