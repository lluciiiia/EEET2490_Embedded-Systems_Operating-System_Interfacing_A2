#include "../header/util.h"

void copy_string(char *dest, const char *src)
{
    while (*src)
    {
        *dest++ = *src++;
    }
    *dest = '\0'; // Add null terminator to the end of the destination string
}

int compare_string(const char *str1, const char *str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return (*str1 == '\0' && *str2 == '\0') ? 0 : (*str1 - *str2);
}


int compare_string_start(const char *str, const char *prefix)
{
    while (*prefix && *str)
    {
        if (*str != *prefix)
        {
            return -1;
        }
        str++;
        prefix++;
    }
    // If prefix is empty, return true
    if (*prefix == '\0')
        return 0;

    return -1;
}

