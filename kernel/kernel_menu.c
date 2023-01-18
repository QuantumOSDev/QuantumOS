#include <quantum/kernel_menu.h>
#include <quantum/init.h>

#include <drivers/keyboard.h>
#include <drivers/vesa.h>

#include <core/print.h>
#include <sys/acpi.h>

static int menu_position_selected = 0;
static int all_menu_position      = 4;

kernel_menu_selection_item_t items[4] = {
    {
        .name = "Boot into userspace"
    },
    {
        .name = "Boot into kernel-mode"
    },
    {
        .name = "Reboot"
    },   
    {
        .name = "Shutdown"
    },    
};

void draw_menu_title()
{
    print_set_color_bg(0, 0, 0);
    print_set_color(255, 255, 255);
    set_print_x((get_screen_x() / 2) - ((9 * 9) / 2));
    set_print_y(get_screen_y() / 4);
    printf("Quantum OS");
}

void draw_menu_selection_item(int index, int menu_x, int menu_y)
{
    if (menu_position_selected == index)
    {
        vesa_draw_rect(
            menu_x + 3, (menu_y + 3) + (60) * (index + 0),
            390 - 6, 60, 255, 255, 255
        );

        set_print_x(menu_x + 3 + 15);
        set_print_y((menu_y + 3) + ((26) * (index + 1)) + index * 30);

        print_set_color_bg(255, 255, 255);
        print_set_color(0, 0, 0);

        printf(items[index].name);
    }
    else 
    {
        set_print_x(menu_x + 3 + 15);
        set_print_y((menu_y + 3) + ((26) * (index + 1)) + index * 30);

        print_set_color_bg(0, 0, 0);
        print_set_color(255, 255, 255);

        printf(items[index].name);
    }

}

void draw_menu_selection_bar()
{
    int selection_bar_height = 3 * 60 + 100;
    int selection_bar_width  = 390;

    int x_start = (get_screen_x() / 2) - (selection_bar_width / 2);
    int y_start = (get_screen_y() / 4 + 30);

    vesa_draw_line(
        x_start , y_start - 7, 
        x_start + selection_bar_width, y_start - 7,
        255, 255, 255
    );

    for (int i = 0; i < all_menu_position; i++)
        draw_menu_selection_item(i, (get_screen_x() / 2) - (selection_bar_width / 2), (get_screen_y() / 4 + 30));
}

void draw_menu()
{
    draw_menu_title();
    draw_menu_selection_bar();
}

void quantum_menu()
{
    quantum_info(2, " Menu   ", "Waiting for user decision");

    char sc = 0;
    draw_menu();

    for (;;) 
    { 
        sc = keyboard_getsc();
        if (sc == 80)
        {
            menu_position_selected++;
            if (menu_position_selected >= 4)
            {
                menu_position_selected = 0;
            }
            vesa_clear();
            draw_menu();
        }
        else if (sc == 72)
        {
            menu_position_selected--;
            if (menu_position_selected == -1)
            {
                menu_position_selected = 3;
            }
            vesa_clear();
            draw_menu();
        }
        else if (sc == KEYBOARD_ENTER_SC)
        {
            vesa_clear();
            set_default_pos();
            print_set_color_bg(0, 0, 0);
            print_set_color(255, 255, 255);
            switch (menu_position_selected)
            {
            case 0:
                quantum_migrate_to_userspace();
                break;
            case 1:
                quantum_migrate_to_kernel_mode();
                break;
            case 2:
                acpi_reboot();
                break;
            case 3:
                acpi_shutdown();
                break;
            default:
                break;
            }
        }

        // Menu logic
    }   

}