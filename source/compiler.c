#include <stdio.h>

#include "compile.h"
#include "util/dynarr.h"

int main(int argc, char** argv)
{
    return compile("int a = 2;");
}
