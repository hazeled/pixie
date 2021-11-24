#include <stdio.h>

#include "compile.h"
#include "util/dynarr.h"

int main(int argc, char** argv)
{
    /* if (argc != 2) { */
    /*     printf("Source file name required\n"); */
    /*     return 1; */
    /* } */
    
    return compile("int a = 2a;");
}
