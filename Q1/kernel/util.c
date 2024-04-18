#include "../header/util.h"

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

