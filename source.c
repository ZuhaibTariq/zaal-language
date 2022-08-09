#include "stdbool.h"
#include <stdlib.h>

#ifndef __SOURCE__
typedef struct source{
    long size;
    long current;
    char* data;
}Source;

Source* new_source(long size, char* data)
{
    Source* source = (Source*) malloc(sizeof(Source));
    source->size = size;
    source->data = data;
    source->current = 0;

    return source;
}

void free_source(Source* ptr)
{
    free(ptr->data);
    free(ptr);
}
#define __SOURCE__
#endif