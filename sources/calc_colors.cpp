#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

#include "calc_colors.h"

int cprintf(const char* color, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    printf("%s",color);
    int ret = vprintf(fmt, args);
    printf("%s", BLACK);
    fflush(stdout);

    return ret;
}

int cputs(const char* color, const char* str)
{
    printf("%s",color);
    int ret = fputs(str, stdout);
    printf("%s", BLACK);

    return ret;
}


