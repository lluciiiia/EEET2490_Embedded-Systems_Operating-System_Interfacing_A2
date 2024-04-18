#include "../header/command.h"

void help_command(char *command)
{
    // Check if the command is "help" without any argument
    if (compare_string(command, "") == 0)
    {
        uart_puts("\nAvailable commands:\n");
        uart_puts("help - Show brief information of all commands\n");
        uart_puts("help <command_name> - Show full information of the command\n");
        uart_puts("clear - Clear screen\n");
        uart_puts("setcolor -t <text color> -b <background color> - Set text color and/or background color\n");
        uart_puts("showinfo - Show board revision and board MAC address\n");
    }
    else
    {
        // Check the command name and display full information accordingly
        if (compare_string(command, "help") == 0)
        {
            uart_puts("\nhelp - Show brief information of all commands\n");
            uart_puts("help <command_name> - Show full information of the command\n");
        }
        else if (compare_string(command, "clear") == 0)
        {
            uart_puts("\nclear - Clear screen\n");
        }
        else if (compare_string(command, "setcolor") == 0)
        {
            uart_puts("\nsetcolor -t <text color> -b <background color> - Set text color and/or background color\n");
        }
        else if (compare_string(command, "showinfo") == 0)
        {
            uart_puts("\nshowinfo - Show board revision and board MAC address\n");
        }
        else
        {
            uart_puts("\nCommand not found\n");
        }
    }
}

void clear_command()
{
    uart_puts("\033[2J\033[H"); // ANSI escape sequence to clear the screen
}

void setcolor_command(char *text_color, char *background_color)
{
    uart_puts("Text color set to ");
    uart_puts(text_color);
    uart_puts(" and background color set to ");
    uart_puts(background_color);
    uart_puts("\n");
}

void showinfo_command()
{
    uart_puts("Board revision: REV1.0\n");
    uart_puts("Board MAC address: AA:BB:CC:DD:EE:FF\n");
}

void execute_command(char *command)
{
    // Check if the command starts with "help"
    if (compare_string_start(command, "help") == 0)
    {
        // Skip "help" and any leading spaces
        char *command_name = command + 4; // "help" has 4 characters
        while (*command_name == ' ')
        {
            command_name++;
        }

        // Pass the command name to help_command
        help_command(command_name);
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
        uart_puts("\nCommand not recognized\n");
    }
}
