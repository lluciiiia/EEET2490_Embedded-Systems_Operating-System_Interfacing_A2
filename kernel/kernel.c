#include "../header/uart0.h"
#include "../header/util.h"
#include "../header/command.h"
#include "../header/ui.h"

// A list of available commands for autocompletion
const char *command_list[] = {
    "help",
    "clear",
    "setcolor",
    "showinfo",
    "setbaudrate",
    "setdatabits",
    "setstopbits",
    "setparity",
    "sethandshaking"};

const int num_commands = sizeof(command_list) / sizeof(command_list[0]);

void main()
{

    // intitialize UART
    uart_init();

    display_welcome_msg();
    display_prompt();

    // Command buffer
    char command_buffer[MAX_COMMAND_LENGTH];
    int buffer_index = 0;
    int auto_complete_index = 0;

    while (1)
    {
        // Read each char
        char c = uart_getc();

        // Handle different keys
        if (c == '\n') // Enter key
        {
            command_buffer[buffer_index] = '\0'; // Null-terminate the command buffer
            execute_command(command_buffer);     // Execute the command
            // Reset buffer index for next command
            buffer_index = 0;
            // Display prompt for next command
            display_prompt();
        }
        else if (c == 0x08 || c == 0x7F) // Delete or backspace key
        {
            // Check if buffer_index is greater than 0 to avoid underflow
            if (buffer_index > 0)
            {
                // Move the cursor back one position
                uart_sendc(c);

                // Remove the last character from the command buffer
                buffer_index--;
            }
        }
        else if (c == '\t') // TAB key for autocompletion
        {
            reset_command_line();

            for (int i = 0; i < MAX_COMMAND_LENGTH; i++)
            {
                command_buffer[i] = '\0';
            }

            buffer_index = 0;

            const char *suggestion = command_list[auto_complete_index];

            for (int i = 0; suggestion[i] != '\0'; i++)
            {
                uart_sendc(suggestion[i]);
                command_buffer[buffer_index++] = suggestion[i];
            }

            auto_complete_index++;
            if (auto_complete_index == num_commands)
                auto_complete_index = 0;
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
