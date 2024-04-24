#include "../header/command.h"

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
    char *text_color = (void *)0;
    char *background_color = (void *)0;
    int color_found = 0;

    while (*token != '\0')
    {
        uart_puts("*");
        // Check if the token starts with "-t"
        if (token[0] == '-' && token[1] == 't')
        {

            token += 2;

            while (*token == ' ')
            {
                token++;
            }

            if (*token == '\0')
            {
                uart_puts("\nInvalid command! Please specify a color after -t.\n");
                uart_puts("Loop Done\n");
                return;
            }

            text_color = token;

            while (*token != ' ' && *token != '\0')
            {
                token++;
            }

            *token = '\0';
            color_found = 1;
        }
        // Check if the token starts with "-b"
        else if (token[0] == '-' && token[1] == 'b')
        {

            token += 2;

            while (*token == ' ')
            {
                token++;
            }

            if (*token == '\0')
            {
                uart_puts("\nInvalid command! Please specify a color after -b.\n");
                uart_puts("Loop Done\n");
                return;
            }

            background_color = token;

            while (*token != ' ' && *token != '\0')
            {
                token++;
            }

            *token = '\0';
            color_found = 1;
        }
        token++;
    }

    if (color_found)
    {
        toUppercase(text_color);
        toUppercase(background_color);
        change_color(text_color, background_color);
        uart_puts("\nColor has changed!!\n");
        return;
    }

    uart_puts("\nInvalid command! Please specify at least one color.\n");
    uart_puts("Loop Done\n");
}

void showinfo_command()
{
    get_board_info();
}