#include <stdio.h>

#include "calc_hash.h"

unsigned long CalcCountHashDjb2(const char* str)
{
    unsigned long hash = 5381;

    while (*str != '\0')
    {
        hash = ((hash << 5) + hash) + (unsigned long)(*str);
        str++;
    }

    return hash;
}
