#include "stdbool.h"
#include <stdlib.h>

#ifndef __SOURCE_H_INCLUDED__
#define __SOURCE_H_INCLUDED__

typedef struct source{
    unsigned long size;
    unsigned long current;
    unsigned long line;
    const char* data;
}Source;

#endif