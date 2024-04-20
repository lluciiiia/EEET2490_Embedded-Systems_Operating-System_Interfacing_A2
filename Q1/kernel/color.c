#include "../header/color.h"

int change_color(char *text, char *background)
{
    uart_puts(text);
    uart_puts(background);

    uart_puts(RED_TEXT);
    uart_puts(WHITE_BACKGROUND);

    return 0;
}
