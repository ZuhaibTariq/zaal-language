#include "stdbool.h"
#include <stdlib.h>

#ifndef __SOURCE_H_INCLUDED__
#define __SOURCE_H_INCLUDED__

typedef struct source{
    long size;
    long current;
    const char* data;
}Source;

#endif