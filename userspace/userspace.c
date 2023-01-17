#include <userspace/userspace.h>
#include <userspace/syscalls.h>
#include <userspace/usermode.h>

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

#include <fs/vfs.h>

void print_command(char* command, char* description) 
{
    print_set_color(100, 149, 237);
    printf("%s", command);
    print_set_color(255, 255, 255);
    printf(":%s\n", description);
}

void print_error(char* name, char* error_msg, ...)
{
    print_set_color(180, 0, 0);
    printf("%s", name);
    print_set_color(255, 255, 255);
    printf(": ");
    va_list arg;
    va_start(arg, error_msg);
    printf_va(error_msg, arg);
    va_end(arg);
    insert_newline();
}

/*
int shell_entry() 
{
    char* directory = "/";

    while (1)
    {
        print_set_color(0, 210, 0);
        printf("%s", directory);
        print_set_color(255, 255, 255);
        printf(" $ ");

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
                print_set_color(184, 134, 11);
                printf("Help page:\n");
                print_set_color(255, 255, 255);
                print_command("touch", " Create new file.");
                print_command("mkdir", " Create new directory.");
                print_command("cat", "   Read a file.");
                print_command("ls", "    Show folders and files in actuall directory.");
                print_command("ewq", "   Program to write into file");
                print_command("cd", "    Change actuall directory.");
                print_command("help", "  Display this help window.");
            }
            else if (strcmp(__basecmd, "touch") == 0)
            {
                __basecmd = strtok(NULL, " ");
                if (__basecmd == NULL) 
                {
                    print_error("touch", "expected filename to create, touch [filename]");
                    break;
                }

                __vfs_dir_t* dir = vfs_get_dir_by_path(vfs_get_rootfs(), directory);
                if (vfs_create_file(dir, __basecmd) == NULL) 
                {
                    print_error("touch", "could not create file with filename \"%s\"", __basecmd);
                    break;
                }
                
                break;
            }
            else if (strcmp(__basecmd, "mkdir") == 0)
            {
                __basecmd = strtok(NULL, " ");
                if (__basecmd == NULL) 
                {
                    print_error("mkdir", "expected filename to create, mkdir [directory_name]");
                    break;
                }

                __vfs_dir_t* dir = vfs_get_dir_by_path(vfs_get_rootfs(), directory);
                if (vfs_create_dir(dir, __basecmd) == NULL) 
                {
                    print_error("mkdir", "could not create directory with name \"%s\"", __basecmd);
                    break;
                }
                
                break;
            }
            else if (strcmp(__basecmd, "ls") == 0)
            {
                __vfs_dir_t* dir = vfs_get_dir_by_path(vfs_get_rootfs(), directory);
                for (int i = 0; i < dir->__num_directories; i+=2)
                {
                    print_set_color(0, 0, 220);
                    printf("%s\n", dir->__directories[i]->__name);
                }
                for (int i = 0; i < dir->__num_files; i++)
                {
                    print_set_color(235, 235, 235);
                    printf("%s\n", dir->__files[i]->__name);
                }
            }
            else if (strcmp(__basecmd, "cat") == 0)
            {
                __basecmd = strtok(NULL, " ");
                if (__basecmd == NULL) 
                {
                    print_error("cat", "expected filename to be readed, cat [filename]");
                    break;
                }

                __vfs_dir_t* dir = vfs_get_dir_by_path(vfs_get_rootfs(), directory);
                __vfs_file_t* file = vfs_get_file_in_dir(dir, __basecmd);

                if (file == NULL) 
                {
                    print_error("cat", "could not find file to be readed, cat [filename]");
                    break;
                }

                printf("%s", file->__data);
            }
            else if (strcmp(__basecmd, "eqw") == 0)
            {
                __basecmd = strtok(NULL, " ");
                if (__basecmd == NULL) 
                {
                    print_error("eqw", "expected filename to be writted, eqw [filename]");
                    break;
                }

                __vfs_dir_t* dir = vfs_get_dir_by_path(vfs_get_rootfs(), directory);
                __vfs_file_t* file = vfs_get_file_in_dir(dir, __basecmd);

                if (file == NULL) 
                {
                    print_error("eqw", "could not find file to be writted, eqw [filename]");
                    break;
                }

                char* buf = keyboard_getchar_until(27);
                vfs_write(buf, sizeof(char), strlen(buf), file);
                // file->__data = buf;
            }
            else if (strcmp(__basecmd, "cd") == 0)
            {
                __basecmd = strtok(NULL, " ");
                if (__basecmd == NULL) 
                {
                    print_error("cd", "expected directory path, cd [directory path]");
                    break;
                }

                printf("%d\n", vfs_get_dir_by_path(vfs_get_rootfs(), __basecmd));
                if (vfs_get_dir_by_path(vfs_get_rootfs(), __basecmd) == NULL)
                {
                    print_error("cd", "could not find directory path \"%s\" , cd [directory path]", __basecmd);
                    break; 
                }
                else
                {
                    directory = __basecmd;
                }
            }
            else
            {
                printf("Unknown command: \"%s\"\n", __basecmd);
                break;
            }

            __basecmd = strtok(NULL, " ");
        }
    }
}
*/

void userspace_initialize(void) 
{
    vesa_clear();
    set_default_pos();

    printf("Booting into userspace...\n");
    // switch_to_user_mode();

    // process_spawn((void*)shell_entry, PROCESS_MODE_KERNEL, 10000, PROCESS_RUNNING, 1);

    for (;;) {}
}