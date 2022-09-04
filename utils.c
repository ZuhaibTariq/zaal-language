#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Includes/utils.h"

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
    }
    if (str[line_start] == '\n') line_start++;
    printf("%.*s", (int) current - line_start, str + line_start);
    if (str[current-1] != '\n') printf("\n");
    printf("%*s^\n", start - line_start, "");
}

long read_source_file(char* file, char** buffer)
{
    // open an existing file for reading
    FILE* fptr = fopen(file, "r");

    // exit if the file does not exist
    if(fptr == NULL) exit(1);

    // Get the number of bytes
    fseek(fptr, 0L, SEEK_END);
    long numbytes = ftell(fptr);

    // reset the file position indicator to the beginning of the file
    fseek(fptr, 0L, SEEK_SET);

    // grab sufficient memory for the buffer to hold the text
    *buffer = (char*)calloc(numbytes, sizeof(char)); //responsibility of the caller to free this memory
    
    // memory error
    if (*buffer == NULL) exit(2);
    
    // copy all the text into the buffer
    fread(*buffer, sizeof(char), numbytes, fptr);
    fclose(fptr);

    //return a source object with buffer as data and size as its length
    return numbytes;

}