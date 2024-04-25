#include "../header/ui.h"

void display_welcome_msg()
{
    // Print an ASCII art welcome message
    uart_puts("\n\n########:'########:'########:'########:::::'#######::'##:::::::::'#######::::'#####:::\n"
              "##.....:: ##.....:: ##.....::... ##..:::::'##.... ##: ##:::'##::'##.... ##::'##.. ##::\n"
              "##::::::: ##::::::: ##:::::::::: ##:::::::..::::: ##: ##::: ##:: ##:::: ##:'##:::: ##:\n"
              "######::: ######::: ######:::::: ##::::::::'#######:: ##::: ##::: ########: ##:::: ##:\n"
              "##...:::: ##...:::: ##...::::::: ##:::::::'##:::::::: #########::...... ##: ##:::: ##:\n"
              "##::::::: ##::::::: ##:::::::::: ##::::::: ##::::::::...... ##::'##:::: ##:. ##:: ##::\n"
              "########: ########: ########:::: ##::::::: #########::::::: ##::. #######:::. #####:::\n"
              "........::........::........:::::..::::::::.........::::::::..::::.......:::::.....::::\n\n");

    uart_puts("'########:::::'###::::'########::'########:::::'#######:::'######::\n"
              " ##.... ##:::'## ##::: ##.... ##: ##.....:::::'##.... ##:'##... ##:\n"
              " ##:::: ##::'##:. ##:: ##:::: ##: ##:::::::::: ##:::: ##: ##:::..::\n"
              " ########::'##:::. ##: ########:: ######:::::: ##:::: ##:. ######::\n"
              " ##.... ##: #########: ##.. ##::: ##...::::::: ##:::: ##::..... ##:\n"
              " ##:::: ##: ##.... ##: ##::. ##:: ##:::::::::: ##:::: ##:'##::: ##:\n"
              " ########:: ##:::: ##: ##:::. ##: ########::::. #######::. ######::\n"
              "........:::..:::::..::..:::::..::........::::::.......::::......:::\n\n");

    uart_puts("Developed By Seokyung Kim - S3939114.\n\n");
}

void display_prompt()
{
    uart_puts("KimOS> ");
}

void display_start(char *title)
{
    uart_puts("\n\n........................................ ");
    uart_puts(title);
    uart_puts(" ........................................\n\n");
}

void display_end()
{
    uart_puts("\n\n.................................................................................................\n\n");
}

void reset_command_line()
{
    // Clear the current line and move cursor to the beginning
    uart_sendc('\033'); // Escape character
    uart_sendc('[');    // Control sequence introducer
    uart_sendc('2');    // Parameter for clearing entire line
    uart_sendc('K');    // Command to clear entire line
    uart_sendc('\r');   // Carriage return to move cursor to the beginning of the line

    // Display the prompt for the next command
    display_prompt();
}