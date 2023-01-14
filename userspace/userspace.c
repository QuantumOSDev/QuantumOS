#include <userspace/userspace.h>
#include <userspace/syscalls.h>
#include <userspace/usermode.h>

#include <drivers/sound_blaster.h>
#include <drivers/keyboard.h>
#include <drivers/vesa.h>

#include <core/stdlib.h>
#include <core/string.h>
#include <core/print.h>

#include <sys/process.h>
#include <sys/memory.h>

int shell_entry() 
{
    while (1)
    {
        print_set_color(0, 210, 0);
        printf("/");
        print_set_color(255, 255, 255);
        printf(" $");

        char *__command = keyboard_getchar_until('\n');
        if (!__command)
        {
            printf("err!\n");
        }
        char *__basecmd = strtok(__command, " ");

        printf("\n");

        while (__basecmd != NULL)
        {
            if (strcmp(__basecmd, "help") == 0)
            {
                printf("Help page:\n\
help:       Display this help window.\n");
            }
            else
            {
                printf("Invalid command: [%s]\n", __basecmd);
            }

            __basecmd = strtok(NULL, " ");
        }
    }
}

void userspace_initialize(void) 
{
    vesa_clear();
    set_default_pos();

    printf("Booting into userspace...\n");
    switch_to_user_mode();

    process_spawn((void*)shell_entry, PROCESS_MODE_KERNEL, 10000, PROCESS_RUNNING, 1);

    for (;;) {}
}