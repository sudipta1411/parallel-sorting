#include<stdio.h>
#include<time.h>
#include<math.h>
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

/*http://www.geeksforgeeks.org/reservoir-sampling
 * Reservoir Sampling*/
static int_array_t* get_random_sample(int_array_t* array, size_t k)
{
    if(!array || k<=0 ||k> array->len)
        return NULL;
    int_array_t* reservoir = int_array_create(k);
    if(!reservoir)
        return NULL;
    unsigned long i;
    int buf, j;
    for(i=0; i<k; i++)
    {
        buf = int_array_get(array, i);
        int_array_set(reservoir, buf, i);
    }
    srand(time(NULL));
    for(; i < array->len; i++)
    {
        j = rand() % (i + 1);
        if(j < k)
        {
            buf = int_array_get(array, i);
            int_array_set(reservoir, buf, j);
        }
    }
    return reservoir;
}

int_array_t* parallel_prefix_sum(int_array_t* array)
{
    if(!array)
        return NULL;
    size_t len = array->len, i, iter;
    int height = log2(len), h;
    printf("height : %d\n", height);
    int* sum[height+1];
    int* pps[height+1];
    sum[0] = (int*)malloc(len*sizeof(int));
#ifdef OMP
#pragma omp parallel shared(sum, len, array) private(i)
    {
#pragma omp for schedule(static)
#endif
        for(i=0; i<len; i++)
        {
            sum[0][i] = int_array_get(array, i);
            //fprintf(stdout, "[%d] : %d\n", omp_get_thread_num(), sum[0][i]);
        }
#ifdef OMP
    }
#endif
    for(h=1; h <= height; h++)
    {
        iter = len / pow(2, h);
        sum[h] = (int*) malloc(iter*sizeof(int));
#ifdef OMP
#pragma omp parallel \
        shared(array, sum, iter, h) \
        private(i)
        {
#pragma omp for schedule(static)
#endif
            for(i=0; i<iter; i++)
            {
                sum[h][i] = sum[h-1][2*i] + sum[h-1][2*i + 1];
                //fprintf(stdout, "[%d] : sum[%d][%lu] %d\n", omp_get_thread_num(), h, i, sum[h][i]);
            }
#ifdef OMP
        }
#endif
    }
    //fprintf(stdout, "sum : %d\n", sum[height][0]);
    pps[height] = (int*) malloc(sizeof(int));
    pps[height][0] = 0;
    for(h = height - 1; h <= 0; h++)
    {
        iter = len / pow(2, h);
        pps[h] = (int*)malloc(iter*sizeof(int));
#ifdef OMP
#pragma omp parallel \
        shared(sum, pps,iter, h) \
        private(i)
        {
#pragma omp for schedule(static)
            for(i=0; i<iter; i++)
            {
                if(i % 2 == 0) /*Left child*/
                {
                    pps[h][i] = pps[h + 1][i / 2];
                } else
                {
                    //pps[h][i] = pps
                }
            }
#endif
#ifdef OMP
        }
#endif
    }
    return NULL;
}

void run_parallel_partition_sort(size_t size)
{
    size_t resv_size;
    create(size);
    fprintf(stdout, "Original Array\n");
    resv_size = ceil(sqrt(size));
    set_with_rand(array);
    print(array);
    fprintf(stdout, "Reservoir size %lu\n", resv_size);
    int_array_t* reservoir = get_random_sample(array, resv_size);
    fprintf(stdout, "Original Array\n");
    print(reservoir);
    int_array_t* pps = parallel_prefix_sum(reservoir);
    fprintf(stdout, "Parallel prefix sum\n");
    print(pps);
    int_array_destroy(&pps);
    int_array_destroy(&reservoir);
    destroy();
}
