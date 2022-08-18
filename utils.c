#include <stdio.h>
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

void log_line(const char* str, int start, int current)
{   int line_start = 0;
    char ch;
    for(line_start = start; str[line_start] != '\n' && line_start > 0; line_start--)
    {
        ch = str[line_start];
        //
    }
    if (str[line_start] == '\n') line_start++;
    printf("%.*s", (int) current - line_start, str + line_start);
    if (str[current-1] != '\n') printf("\n");
    printf("%*s^\n", start - line_start, "");
}
