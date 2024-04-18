#include "../header/uart0.h"
#include "../header/util.h"

void help_command();

void clear_command();

void setcolor_command(char *text_color, char *background_color);

void showinfo_command();

void execute_command(char* command);