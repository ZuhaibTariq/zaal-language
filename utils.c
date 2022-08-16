#include <stdlib.h>
#include <string.h>
#include "utils.h"

char* slice(const char *str, size_t start, size_t end)
{
    char* sub_str = calloc(end - start + 1, sizeof(char)); // responsibilty of the caller to free this memory
    strncpy(sub_str, str + start, end - start);
    sub_str[end-start] = '\0';
    return sub_str;
}