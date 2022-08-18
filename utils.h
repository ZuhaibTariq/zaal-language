#include <stddef.h>

#ifndef __UTILS_H_INCLUDED__
#define __UTILS_H_INCLUDED__

char* slice(const char *str, size_t start, size_t end);

void log_line(const char* str, int start, int current);

#endif