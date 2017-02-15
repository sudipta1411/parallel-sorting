#include<stdio.h>
#include<time.h>
#include<omp.h>
#include"merge_sort.h"
#include"array.h"

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

static void set_with_rand(int_array_t* array)
{
    size_t i;
    int r;
    srand((unsigned)time(NULL));
    for(i=0; i<array->len; i++)
    {
        r = rand();
        int_array_set(array, r, i);
    }
}
static void print(int_array_t* array)
{
    if(array)
    {
        size_t i;
        for(i=0; i<array->len; i++)
        {
            fprintf(stdout, "%d ", int_array_get(array, i));
        }
        fprintf(stdout, "\n");
    }
}

static void parallel_merge(int_array_t* array, size_t start, size_t end, comparator_int comp)
{
    if(!array) return;
    if(end - start == 1)
        int_swap(&(array->ar[start]), &(array->ar[end]));
}

void run_parallel_merge_sort(size_t size)
{
    create(size);
    set_with_rand(array);
    print(array);
    parallel_merge(array, 0, 1, &int_comp);
    print(array);
    destroy();
}
