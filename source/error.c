#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "error.h"

void throw_compiler_error(const char* error, ...)
{
    va_list args;
    va_start(args, error);
    printf("**ERROR**\n");
    vprintf(error, args);
    exit(-1);
}
