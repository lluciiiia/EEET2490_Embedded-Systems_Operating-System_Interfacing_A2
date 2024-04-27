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
    else if (compare_string_start(command, "setbaudrate") == 0)
    {
        set_baud_rate_command(command + strlen("setbaudrate"));
    }
    else if (compare_string_start(command, "setdatabits") == 0)
    {
        set_data_bits_command(command + strlen("setdatabits"));
    }
    else if (compare_string_start(command, "setstopbits") == 0)
    {
        set_stop_bits_command(command + strlen("setstopbits"));
    }
    else if (compare_string_start(command, "setparity") == 0)
    {
        // Skip "setparity" and any leading spaces
        char *command_name = command + 9; // "setparity" has 9 characters
        while (*command_name == ' ')
        {
            command_name++;
        }
        set_parity_command(command_name);
    }
    else if (compare_string_start(command, "sethandshaking") == 0)
    {
        // Skip "sethandshaking" and any leading spaces
        char *command_name = command + 14; // "sethandshaking" has 14 characters
        while (*command_name == ' ')
        {
            command_name++;
        }
        set_handshaking_command(command_name);
    }
    else
    {
        uart_puts("\nCommand not recognized\n");
    }
}

void help_command(char *command)
{
    uart_puts("\n\n");

    // Check if the command is "help" without any argument
    if (compare_string(command, "") == 0)
    {
        uart_puts("Command                                                      Description\n");
        uart_puts("---------------------------------------------------------------------------------------------------------------------\n");
        uart_puts("help                                                         Show brief information of all commands\n\n");
        uart_puts("help <command_name>                                          Show full information of the command\n\n");
        uart_puts("clear                                                        Clear screen\n\n");
        uart_puts("setcolor -t <text color> -b <background color>               Set text color and/or background color\n");
        uart_puts("                                                             Colors: black, red, yellow, blue, purple, cyan, white\n\n");
        uart_puts("showinfo                                                     Show board revision and board MAC address\n\n");
        uart_puts("setbaudrate <baud rates>                                     Set desired baud rates\n");
        uart_puts("                                                             Baud rates: 9600, 19200, 38400, 57600, 115200\n\n");
        uart_puts("setdatabits <the number of data bits>                        Set the number of data bits\n");
        uart_puts("                                                             Data bits: 5, 6, 7, or 8\n\n");
        uart_puts("setstopbits <1 or 2>                                         Select between one or two stop bits\n");
        uart_puts("                                                             Options: 1 or 2\n\n");
        uart_puts("setparity <none, even or odd>                                Configure parity\n");
        uart_puts("                                                             Options: none, even, or odd\n\n");
        uart_puts("sethandshaking                                               Set handshaking between CTS and RTS\n");
        uart_puts("                                                             Options: on, off\n\n");
    }
    else
    {
        // Check the command name and display full information accordingly
        if (compare_string(command, "help") == 0)
        {
            uart_puts("help - Show brief information of all commands\n");
            uart_puts("help <command_name> - Show full information of the command\n\n");
        }
        else if (compare_string(command, "clear") == 0)
        {
            uart_puts("clear - Clear screen\n\n");
        }
        else if (compare_string(command, "setcolor") == 0)
        {
            uart_puts("setcolor -t <text color> -b <background color> - Set text color and/or background color of the console.\n");
            uart_puts("You have following color options: black, red, yellow, blue, purple, cyan, white.\n\n");
        }
        else if (compare_string(command, "showinfo") == 0)
        {
            uart_puts("showinfo - Show board revision and board MAC address\n\n");
        }
        else if (compare_string(command, "setbaudrate") == 0)
        {
            uart_puts("setbaudrate <baud rates> - Set desired baud rates, including 9600, 19200, 38400, 57600, and 115200 bits per second.\n\n");
        }
        else if (compare_string(command, "setdatabits") == 0)
        {
            uart_puts("setdatabits <the number of data bits> - Set the number of data bits (5, 6, 7, or 8).\n\n");
        }
        else if (compare_string(command, "setstopbits") == 0)
        {
            uart_puts("setstopbits <1 or 2> - Select between one or two stop bits.\n\n");
        }
        else if (compare_string(command, "setparity") == 0)
        {
            uart_puts("setparity <none, even or odd> - Configure none, even, and odd parity.\n\n");
        }
        else if (compare_string(command, "sethandshaking") == 0)
        {
            uart_puts("sethandshaking <on or off> - Set handshaking between CTS and RTS.\n\n");
        }
        else
        {
            uart_puts("Command not found.\n\n");
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
        uart_puts("\nColor has changed!!\n\n");

        return;
    }

    uart_puts("\nInvalid command! Please specify at least one color.\n\n");
}

void showinfo_command()
{
    display_start("BOARD INFORMATION");
    get_mac_address_info();
    uart_puts("\n\n");
    get_revision_info();
    display_end();
}