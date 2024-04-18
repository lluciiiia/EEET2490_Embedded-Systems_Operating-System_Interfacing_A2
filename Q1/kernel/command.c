#include "../header/command.h"

void help_command()
{
    uart_puts("Help command executed\n");
}

void clear_command()
{
    uart_puts("\033[2J\033[H"); // ANSI escape sequence to clear the screen
}

void setcolor_command(char *text_color, char *background_color)
{
    uart_puts("Setcolor command executed\n");
}

void showinfo_command()
{
    uart_puts("Showinfo command executed\n");
}

void execute_command(char *command)
{
    if (compare_string(command, "help") == 0)
    {
        help_command();
    }
    else if (compare_string(command, "clear") == 0)
    {
        clear_command();
    }
    else if (compare_string(command, "setcolor") == 0)
    {
        setcolor_command("default", "default"); // Default colors
    }
    else if (compare_string(command, "showinfo") == 0)
    {
        showinfo_command();
    }
    else
    {
        uart_puts("Command not recognized\n");
    }
}
