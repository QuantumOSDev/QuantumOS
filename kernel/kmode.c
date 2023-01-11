#include <drivers/keyboard.h>
#include <drivers/acpi.h>

#include <core/string.h>
#include <core/print.h>

#include <sys/kmode.h>

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
                printf("Help commands!\n");
            }
            else if (strcmp(__basecmd, "reboot") == 0)
            {
                acpi_reboot();
            }
            else
            {
                printf("Invalid command: [%s]\n", __basecmd);
            }

            __basecmd = strtok(NULL, " ");
        }
    }
}