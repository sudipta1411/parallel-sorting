#include<stdio.h>
#include"merge_sort.h"
#include"array.h"

array_impl(int);

static int_array_t* array;

static inline void int_swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

static int int_comp(int* a, int* b)
{
    int ret = 0;
    if(*a > *b)
        ret = 1;
    else if(*a < *b)
    {
        ret = -1;
        int_swap(a,b);
    }
    return ret;
}

static void create(size_t size)
{
    array = int_array_create(size);
    if(!array)
        fprintf(stderr, "Memory allocation error!\n");
}

static void destroy()
{
    int_array_destroy(&array);
}

void run_parallel_merge_sort(size_t size)
{
    create(size);
    destroy();
}
