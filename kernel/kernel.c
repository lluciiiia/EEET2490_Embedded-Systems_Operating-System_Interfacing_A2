#include "../header/uart0.h"
#include "../header/util.h"
#include "../header/command.h"
#include "../header/ui.h"

#define HISTORY_SIZE 10

// Command buffer
char command_buffer[MAX_COMMAND_LENGTH];
int buffer_index = 0;
int auto_complete_index = 0;

// Comman history
char command_history[HISTORY_SIZE][MAX_COMMAND_LENGTH];
int history_count = 0;
int history_index = 0;

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

    uart_init();

    display_welcome_msg();
    display_prompt();

    while (1)
    {
        // Read each char
        char c = uart_getc();

        // Handle different keys
        if (c == '\n') // Enter key
        {
            // Add the command to history
            add_to_history(command_buffer);

            // Null-terminate the command buffer
            command_buffer[buffer_index] = '\0';

            // Execute the command
            execute_command(command_buffer);

            // Reset indexes for next command
            buffer_index = 0;
            history_index = history_count - 1;
            auto_complete_index = 0;

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

            clear_buffer();

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
        else if (c == '_') // UP arrow key
        {
            if (history_index >= 0)
            {
                display_history();
                if (history_index != 0)
                    history_index--;
            }
        }
        else if (c == '+') // DOWN arrow key
        {
            if (history_index <= history_count - 1)
            {
                display_history();
                if (history_index != history_count - 1)
                    history_index++;
            }
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

void clear_buffer()
{
    for (int i = 0; i < MAX_COMMAND_LENGTH; i++)
    {
        command_buffer[i] = '\0';
    }

    buffer_index = 0;
}

void add_to_history(const char *command)
{

    // Update history count
    if (history_count >= HISTORY_SIZE)
    {
        // Shift commands in history to make space for the new command
        for (int i = HISTORY_SIZE - 1; i > 0; i--)
        {
            copy_string(command_history[i], command_history[i - 1]);
        }
        copy_string(command_history[history_count], command);
    }
    else
    {
        copy_string(command_history[history_count], command);
        history_count++;
    }

    history_index = history_count - 1;
}

void display_history()
{
    reset_command_line();

    clear_buffer();

    const char *history = command_history[history_index];

    for (int i = 0; history[i] != '\0'; i++)
    {
        command_buffer[buffer_index++] = history[i];
    }

    uart_send_string(history);
}