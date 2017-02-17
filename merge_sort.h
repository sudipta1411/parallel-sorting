#ifndef _MERGE_SORT_H
#define _MERGE_SORT_H
#include<stddef.h>

#define SWAP(x, y) do { \
    typeof(x) t = x;\
    x = y; y = t; } \
while (0)

void run_parallel_merge_sort(size_t);

#endif /*_MERGE_SORT_H*/
