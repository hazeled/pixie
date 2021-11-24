#include <stdio.h>
#include <stdarg.h>

void throw_compiler_error(const char* error, ...)
{
    va_list args;
    va_start(args, error);
    printf("**ERROR**");
    vprintf(error, args);
    exit(-1);
}
