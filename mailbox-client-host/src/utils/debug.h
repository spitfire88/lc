#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#define DEBUG 1

#if DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

void log_error(const char *message);
void log_info(const char *message);

#endif // DEBUG_H