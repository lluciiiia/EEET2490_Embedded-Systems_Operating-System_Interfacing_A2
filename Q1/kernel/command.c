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

void setcolor_command(char *command)
{
    char *token = command;
    char *text_color;
    char *background_color;

    while (*token != '\0')
    {
        // Check if the token is "-t"
        if (compare_string(token, "-t") == 0)
        {
            uart_puts("\n-t found\n");

            token += 2;

            while (*token == ' ')
            {
                token++;
            }

            text_color = token;

            while (*token != ' ' && *token != '\0')
            {
                token++;
            }

            *token = '\0';
        }
        // Check if the token is "-b"
        else if (compare_string(token, "-b") == 0)
        {

            uart_puts("\n-b found\n");
            token += 2;

            while (*token == ' ')
            {
                token++;
            }

            background_color = token;
            while (*token != ' ' && *token != '\0')
            {
                token++;
            }

            *token = '\0';
        }
        token++;
    }

    if (text_color != '\0' || background_color != '\0')
    {
        change_color(text_color, background_color);
        uart_puts("\nColor has changed!!\n");
        return;
    }

    uart_puts("\nInvalid command! Please specify at least one color.\n");
    uart_puts("\nLoop Done\n");
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
    else if (compare_string_start(command, "setcolor") == 0)
    {
        // Skip "help" and any leading spaces
        char *color_command = command + 8; // "help" has 8 characters
        while (*color_command == ' ')
        {
            color_command++;
        }

        setcolor_command(color_command);
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
