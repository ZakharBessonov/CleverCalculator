#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_search.h"

size_t BinarySearch(void* wanted, void* first, void* last, size_t size, int (*comparator) (const void*, const void*))
{
    size_t L = (size_t)first;
    size_t R = (size_t)last;
    size_t middle = (size_t)first;

    if ((R - L == 0) || comparator((void*)wanted, (void*)middle) < 0)
    {
        return 0;
    }

    while (R - L > size)
    {
        middle = L + (((R - L) / size) / 2) * size;

        if (comparator((void*)wanted, (void*)middle) >= 0)
        {
            L = middle;
        }
        else
        {
            R = middle;
        }
    }

    return (R - (size_t)first) / size;
}       //NOTE Использовать бин поиск для односимвольных переменных - полный бред. Выигрыша нет, всё равно мы вставляем
        // переменные за линейное время

ssize_t FindElemInSortedArray(void* wanted, void* first, size_t count,
                             size_t size, int (*comparator) (const void*, const void*))
{
    void* last = (void*)((size_t)first + count * size);
    size_t indexOfWantedElem = BinarySearch(wanted, first, last, size, comparator);

    if (indexOfWantedElem == 0)
    {
        return -1;
    }

    void* suspected = (void*)((size_t)first + (indexOfWantedElem - 1) * size);
    if (comparator(suspected, wanted) == 0)
    {
        return (ssize_t)indexOfWantedElem - 1;
    }

    return -1;
}
