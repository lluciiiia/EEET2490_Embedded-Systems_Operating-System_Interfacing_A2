#include "../header/uart0.h"
#include "../header/util.h"
#include "../header/command.h"
#include "../header/ui.h"

#define MAX_COMMAND_LENGTH 50
#define MAX_HISTORY_SIZE 10


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



