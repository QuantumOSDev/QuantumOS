#include <userspace/userspace.h>
#include <quantum/kmode.h>

#include <drivers/keyboard.h>

#include <core/string.h>
#include <core/print.h>

#include <sys/acpi.h>
#include <sys/cmos.h>

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
reboot:     Reboot the system.\n\
shutdown:   Shutdown the system.\n\
time:       Show current time.\n\
userspace:  Quit kernel-mode and go into userspace!\n\
stz:        Set time zone.\n\
help:       Display this help window.\n");
            }
            else if (strcmp(__basecmd, "reboot") == 0)
            {
                acpi_reboot();
            }
            else if (strcmp(__basecmd, "stz") == 0)
            {
                __basecmd = strtok(NULL, " ");
                if (__basecmd == NULL) 
                {
                    printf("stz: expected --help/-h or time zone argument\n");
                    break;
                }
                else if (strcmp(__basecmd, "--help") == 0 || strcmp(__basecmd, "-h") == 0)
                {
                    printf("stz - kernel mode program to set time zone in quantumos\n\
avaliable time zones:\n\
    - UTC -12:00, UTC -11:00, UTC -10:00, UTC -09:30, UTC -09:00, UTC -08:00, UTC -07:00, UTC -06:00, UTC -05:00, \n\
      UTC -04:00, UTC -03:30, UTC -03:00, UTC -02:00, UTC -01:00\n\
    - UTC 0 \n\
    - UTC +01:00, UTC +02:00, UTC +03:00, UTC +03:30, UTC +04:00, UTC +04:30, UTC +05:00, UTC +05:30, UTC +05:45, \n\
      UTC +06:00, UTC +06:30, UTC +07:00, UTC +08:00, UTC +08:45, UTC +09:00, UTC +09:30, UTC +10:00, UTC +10:30, \n\
      UTC +11:00, UTC +12:00, UTC +12:45, UTC +13:00, UTC +14:00 \n");
                    break;
                }
                
                time_zone_t time_zone = convert_str_to_time_zone(__basecmd);
                if (time_zone == -1) 
                {
                    printf("stz: unknown time zone \"%s\"\n", __basecmd);
                    break;
                }

                set_time_zone(time_zone);
            }
            else if (strcmp(__basecmd, "shutdown") == 0)
            {
                acpi_shutdown();
            }
            else if (strcmp(__basecmd, "userspace") == 0) 
            {
                userspace_initialize();
            }
            else if (strcmp(__basecmd, "time") == 0) 
            {
                date_t date = get_date_cmos();
                printf("%s%d:%s%d:%s%d %d%d:%s%d:%s%d\n", 
                    date.hour <= 9 ? "0" : "",
                    date.hour, date.minute <= 9 ? "0" : "", 
                    date.minute, date.second <= 9 ? "0" : "", 
                    date.second, 
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