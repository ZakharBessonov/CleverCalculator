#ifndef BINARY_SEARCH_HEADER
#define BINARY_SEARCH_HEADER

size_t BinarySearch(void* wanted, void* first, void* last, size_t size, int (*comparator) (const void*, const void*));

ssize_t FindElemInSortedArray(void* wanted, void* first, size_t count,
                              size_t size, int (*comparator) (const void*, const void*));

#endif
