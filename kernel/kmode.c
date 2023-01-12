#include <drivers/keyboard.h>
#include <drivers/acpi.h>
#include <drivers/cmos.h>

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
                printf("Help page:\n\
reboot:      Reboot the system.\n\
help:        Display this help window.\n\
mem:         Get total memory in KB.\n\
time:        Show actual time (UTC).\n\
userspace:   Quit kernel-mode and go into userspace!\n");
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
            else if (strcmp(__basecmd, "time") == 0)
            {
                date_t date = get_date_cmos();
                printf("%s%d:%s%d:%s%d %d%d:%s%d:%s%d\n", 
                    // Time
                    date.hour <= 9 ? "0" : "", date.hour, 
                    date.minute <= 9 ? "0" : "", date.minute, 
                    date.second <= 9 ? "0" : "", date.second,    

                    // Date
                    date.century, 
                    date.year, 
                    date.month <= 9 ? "0" : "", date.month, 
                    date.day <= 9 ? "0" : "", date.day);
            }
            else
            {
                printf("Invalid command: [%s]\n", __basecmd);
            }

            __basecmd = strtok(NULL, " ");
        }
    }
}