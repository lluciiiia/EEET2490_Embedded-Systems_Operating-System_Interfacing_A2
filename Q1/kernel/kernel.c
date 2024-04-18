#include "../header/uart0.h"

#define MAX_COMMAND_LENGTH 50
#define MAX_HISTORY_SIZE 10

// Function prototypes
void display_welcome_msg();
void display_prompt();
void execute_command(char *command);

void main()
{
    // intitialize UART
    uart_init();

    display_welcome_msg();
    display_prompt();

    // Command buffer
    char command_buffer[MAX_COMMAND_LENGTH];
    int buffer_index = 0;

    while (1)
    {
        // Read each char
        char c = uart_getc();

        // Handle different keys
        if (c == '\n')
        {                                        // Enter key
            command_buffer[buffer_index] = '\0'; // Null-terminate the command buffer
            execute_command(command_buffer);     // Execute the command
            // Reset buffer index for next command
            buffer_index = 0;
            // Display prompt for next command
            display_prompt();
        }
        else
        {
            // Regular characters
            // Add the character to the command buffer
            command_buffer[buffer_index++] = c;
            uart_sendc(c); // Echo back to console
        }
    }
}

// Function to display the OS name and prompt
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

// Function to display the OS name and prompt
void display_prompt() {
    uart_puts("MyOS> ");
}

// Function to execute the help command
void help_command()
{
    // Implementation of help command
    uart_puts("Help command executed\n");
}

// Function to execute the clear command
void clear_command()
{
    // Implementation of clear command
    uart_puts("\033[2J\033[H"); // ANSI escape sequence to clear the screen
}

// Function to execute the setcolor command
void setcolor_command(char *text_color, char *background_color)
{
    // Implementation of setcolor command
    uart_puts("Setcolor command executed\n");
}

// Function to execute the showinfo command
void showinfo_command()
{
    // Implementation of showinfo command
    uart_puts("Showinfo command executed\n");
}

// Function to parse and execute the command
void execute_command(char* command) {
    // Implementation to parse and execute the command
    if (compare_string(command, "help") == 0) {
        help_command();
    } else if (compare_string(command, "clear") == 0) {
        clear_command();
    } else if (compare_string(command, "setcolor") == 0) {
        setcolor_command("default", "default"); // Default colors
    } else if (compare_string(command, "showinfo") == 0) {
        showinfo_command();
    } else {
        uart_puts("Command not recognized\n");
    }
}

// Custom function to copy a string
void copy_string(char *dest, const char *src)
{
    while (*src)
    {
        *dest++ = *src++;
    }
    *dest = '\0'; // Add null terminator to the end of the destination string
}

// Custom implementation of compare_string
int compare_string(const char *str1, const char *str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return (*str1 == '\0' && *str2 == '\0') ? 0 : (*str1 - *str2);
}

