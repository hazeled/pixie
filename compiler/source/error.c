#include <stdio.h>
#include <stdarg.h>

void throw_error(const char* error, ...)
{
    va_list args;
    va_start(args, error);
    vprintf(error, args);
    exit(-1);
}
