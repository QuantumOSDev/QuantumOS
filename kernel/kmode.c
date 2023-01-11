#include <drivers/keyboard.h>
#include <drivers/acpi.h>

#include <core/string.h>
#include <core/print.h>

#include <sys/userspace.h>
#include <sys/memory.h>
#include <sys/kmode.h>

#include <quantum/init.h>

void kmode_initialize(void)
{
    while (1)
    {
        printf(KMODE_PROMPT);

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
                printf("Help page:\n                    \
reboot:      Reboot the system.\n                       \
help:        Display this help window.\n                \
mem:         Get total memory.\n                        \
userspace: Quit kernel-mode and go into userspace!\n");
            }
            else if (strcmp(__basecmd, "reboot") == 0)
            {
                acpi_reboot();
            }
            else if (strcmp(__basecmd, "userspace") == 0) 
            {
                userspace_initialize();
            }
            else if (strcmp(__basecmd, "mem") == 0) 
            {
                printf("%d kb\n", get_kernel_mm()->__system.__total_memory / 1024);
            }
            else
            {
                printf("Invalid command: [%s]\n", __basecmd);
            }

            __basecmd = strtok(NULL, " ");
        }
    }
}