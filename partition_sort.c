#include<stdio.h>
#include<time.h>
#include<omp.h>
#include"partition_sort.h"
#include"array.h"
#include"comp.h"

array_impl(int);
define_fptr(int);

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
    {
        ret = 1;
        int_swap(a,b);
    } else if(*a < *b)
    {
        ret = -1;
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

static void set_with_rand(int_array_t* array)
{
    size_t i;
    int r;
    srand((unsigned)time(NULL));
    for(i=0; i<array->len; i++)
    {
        r = rand() % MAX;
        int_array_set(array, r, i);
    }
}
