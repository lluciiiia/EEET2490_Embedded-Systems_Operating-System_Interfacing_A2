#include "../header/command.h"

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
