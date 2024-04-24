#define MAX_COMMAND_LENGTH 50
#define MAX_HISTORY_SIZE 10

void copy_string(char *dest, const char *src);

int compare_string(const char *str1, const char *str2);

int compare_string_start(const char *str, const char *prefix);

void toUppercase(char *str);

char *strcat(char *dest, const char *src);

unsigned int strlen(const char *str);

int atoi(const char *str);