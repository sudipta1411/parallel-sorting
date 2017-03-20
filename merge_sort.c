#include<stdio.h>
#include<time.h>
#include<stdbool.h>
#include<omp.h>
#include"merge_sort.h"
#include"array.h"
#include"comp.h"

array_impl(int);
define_fptr(int);

static int_array_t* array;
static bool debug = false;

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

static void parallel_merge(int_array_t* array, size_t start,
        size_t end, comparator_int comp, size_t v)
{
    size_t st = v << 1;
    size_t i;
    if(st < (end-start))
    {
#ifdef OMP
#pragma omp task
#endif
            parallel_merge(array, start, end, comp, st);
#ifdef OMP
#pragma omp task
#endif
            parallel_merge(array, start + v, end, comp, st);
#ifdef OMP
#pragma omp task
#endif
           for(i = start + v; i< end - v; i += st)
           {
               //fprintf(stdout, "[%d] (%lu, %lu)\n", omp_get_thread_num(), i, i + v);
               comp(&(array->ar[i]), &(array->ar[i+v]));
           }
    } else
    {
        //fprintf(stdout, "[%d] (%lu, %lu)\n", omp_get_thread_num(), start, start + v);
        comp(&(array->ar[start]), &(array->ar[start+v]));
    }
}

static void parallel_merge_sort(int_array_t* array, size_t start,
        size_t end, comparator_int comp)
{
    if(!array) return;
    //fprintf(stdout, "start : %lu  end : %lu\n", start, end);
    if((end - start) >= 1)
    {
        size_t mid = start + (end-start) / 2; //avoids overflow
#ifdef OMP
#pragma omp task
#endif
            parallel_merge_sort(array, start, mid, comp);
#ifdef OMP
#pragma omp task
#endif
            parallel_merge_sort(array, mid+1, end, comp);
#ifdef OMP
#pragma omp task
#endif
            parallel_merge(array, start, end, comp, 1);
    }
}

void run_parallel_merge_sort(size_t size)
{
    create(size);
    set_with_rand(array);
    if(debug) {
        fprintf(stdout, "Original Array\n");
        print(array);
    }
#ifdef OMP
#pragma omp parallel
    {
#endif
        parallel_merge_sort(array, 0, array->len-1, &int_comp);
#ifdef OMP
    }
#endif
    fprintf(stdout, "\n");
    if(debug) {
        fprintf(stdout, "Sorted Array\n");
        print(array);
    }
    destroy();
}
