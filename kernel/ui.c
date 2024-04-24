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