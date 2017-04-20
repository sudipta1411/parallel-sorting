#include<stdio.h>
#include<time.h>
#include<math.h>
#include<omp.h>
#include"partition_sort.h"
#include"array.h"
#include"comp.h"
#include"bin_search_range.h"
#include"stack.h"

#define THRESHOLD_SIZE 20

array_impl(int);
define_fptr(int);

static bool debug = true;
static int_array_t* array;

static inline void int_swap(int* a, int* b)
{
    if(!a || !b) return;
    int t = *a;
    *a = *b;
    *b = t;
}

static int int_comp(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
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
    size_t i, j;
    int r;
    for(i=0; i<array->len; i++)
        int_array_set(array, i, i);
    srand((unsigned)time(NULL));
    for(i=array->len-1; i>0; i--) {
        j = rand() % (i + 1);
        int_swap(&(array->ar[i]), &(array->ar[j]));
    }
    /*for(i=0; i<array->len; i++)
    {
        r = rand() % MAX;
        int_array_set(array, r, i);
    }*/
}

static unsigned long get_next_pow2(unsigned long n)
{
    n--;
    n |= n>>1; n |= n>>2; n |= n>>4;
    n |= n>>8; n |= n>>16; n |= n>>32;
    n++;
    return n;
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
static int_array_t* get_random_sample(int_array_t* array,
        unsigned long start, unsigned long end, size_t k)
{
    if(!array || k<=0 ||k> end - start)
        return NULL;
    /*if(end - start == 2)
        return array;*/
    int_array_t* reservoir = int_array_create(k);
    if(!reservoir)
        return NULL;
    unsigned long i;
    int buf, j;
    for(i=0; i < k; i++)
    {
        buf = int_array_get(array, start + i);
        int_array_set(reservoir, buf, i);
    }
    srand(time(NULL));
    for(; i < end-start; i++)
    {
        j = rand() % (i + 1);
        if(j < k)
        {
            buf = int_array_get(array, start + i);
            int_array_set(reservoir, buf, j);
        }
    }
    return reservoir;
}

static void copy_and_pad(int_array_t* old, int_array_t* new)
{
    if(!old || !new) return;
    if(old->len > new->len) {
        fprintf(stderr, "old array is larger than new array");
        return;
    }
    unsigned long i;
#ifdef OMP
#pragma parallel shared(old, new) private(i)
    {
#pragma omp for schedule(static)
#endif
        for(i=0; i<old->len; i++)
            int_array_set(new, int_array_get(old, i), i);
#ifdef OMP
    }
#endif

#ifdef OMP
#pragma parallel shared(old, new) private(i)
    {
#pragma omp for schedule(static)
#endif
        for(i=old->len; i<new->len; i++)
            int_array_set(new, 0, i);
#ifdef OMP
    }
#endif

}

static int_array_t* parallel_prefix_sum(int_array_t* array)
{
    if(!array)
        return NULL;
    size_t len = array->len, i, iter;
    size_t new_len = get_next_pow2(len);
    int_array_t* ret = int_array_create(len);
    int_array_t* new_array = int_array_create(new_len);
    copy_and_pad(array, new_array);
    int height = log2(new_len), h;
    int* sum[height + 1];
    int* pps[height + 1];
    sum[0] = (int*)malloc(new_len * sizeof(int));
#ifdef OMP
#pragma omp parallel shared(sum, len, array) private(i)
    {
#pragma omp for schedule(static)
#endif
        for(i=0; i<len; i++)
        {
            sum[0][i] = int_array_get(new_array, i);
            //fprintf(stdout, "[%d] : %d\n", omp_get_thread_num(), sum[0][i]);
        }
#ifdef OMP
    }
#endif
    for(h=1; h <= height; h++)
    {
        iter = new_len / pow(2, h);
        sum[h] = (int*) malloc(iter * sizeof(int));
#ifdef OMP
#pragma omp parallel shared(sum, iter, h) private(i)
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
    pps[height] = (int*) malloc(sizeof(int));
    pps[height][0] = 0;
    for(h = height - 1; h >= 0; h--)
    {
        iter = new_len / pow(2, h);
        pps[h] = (int*) malloc(iter*sizeof(int));
#ifdef OMP
#pragma omp parallel shared(sum, pps,iter, h) private(i)
        {
#pragma omp for schedule(static)
#endif
            for(i=0; i<iter; i++)
            {
                if(i % 2 == 0) /*Left child*/ {
                    pps[h][i] = pps[h + 1][i / 2];
                } else /*Right Child*/ {
                    pps[h][i] = pps[h + 1][(i - 1) / 2] + sum[h][i - 1];
                }
            }
#ifdef OMP
        }
#endif
    }
#ifdef OMP
#pragma omp parallel private(i) shared(sum, pps, len)
    {
#pragma omp for schedule(static)
#endif
        for(i=0; i<len; i++)
        {
            pps[0][i] = pps[0][i] + sum[0][i];
            //fprintf(stdout, "[%d] : pps[0][%lu] %d\n", omp_get_thread_num(), i, pps[0][i]);
        }
#ifdef OMP
    }
#endif
#ifdef OMP
#pragma omp parallel private(i) shared(pps, ret, len)
    {
#pragma omp for schedule(static)
#endif
        for(i=0; i<len; i++)
            int_array_set(ret, pps[0][i], i);
#ifdef OMP
    }
#endif
#ifdef OMP
#pragma omp parallel private(h) shared(pps, sum, height)
    {
#pragma omp for schedule(static)
        for(h=0; h<height; h++)
        {
            free(pps[h]);
            free(sum[h]);
            pps[h] = sum[h] = NULL;
        }
#endif
#ifdef OMP
    }
#endif
    int_array_destroy(&new_array);
    return ret;
}

static void copy_from_stack(int_array_t* array, unsigned long start,
        unsigned long end, stack_t* stack)
{
    if(!array || !stack) return;
    int sz = stack_size(stack);
    if(end-start < sz) return;
    unsigned long i = 0;
    for(i = start; i<end; i++) {
        int_array_set(array, pop(stack), i);
    }
    if(stack_size(stack) != 0 )
        fprintf(stdout, "[WARN] stack is not empty");
}

static void parallel_partition_sort(int_array_t* array,
        unsigned long start, unsigned long end)
{
    fprintf(stdout, "[%d] start->%lu, end->%lu\n", omp_get_thread_num(), start, end);
    if(!array || start >= end || end - start == 1)
        return;
    if(end - start <= THRESHOLD_SIZE) {
        qsort(&(array->ar[start]), end, sizeof(int), int_comp);
        return;
    }
    size_t sample_len = ceil(sqrt(end-start));
    int_array_t* sample = get_random_sample(array, start, end, sample_len);
    qsort(sample->ar, sample->len, sizeof(int), int_comp);
    if(debug) {
        fprintf(stdout, "sorted sample\n");
        print(sample);
    }
    unsigned long count;
    stack_t** buckets = NULL;
    buckets = (stack_t**)malloc(sizeof(stack_t*) * (sample->len+1));
#ifdef OMP
#pragma omp parallel private(count) shared(buckets)
    {
#pragma omp for schedule(static)
#endif
        for(count=0; count <= sample->len; count++)
            buckets[count] = create_stack();
#ifdef OMP
    }
#endif
    //count=0;
#ifdef OMP
#pragma omp parallel private(count) shared(buckets, array, sample)
    {
#pragma omp for schedule(static)
#endif
        for(count=0; count < (end - start); count++) {
            if(array->ar[count] < sample->ar[0]) {
                push(buckets[0], array->ar[count]);
            } else if(array->ar[count] > sample->ar[sample->len-1]) {
                push(buckets[sample->len], array->ar[count]);
            } else {
                range_t* rng = bin_search_range(sample->ar, sample->len, array->ar[count]);
                //fprintf(stdout, "[%d] bucket : %d for %dth item->%d\n", omp_get_thread_num(), rng->high, count, array->ar[count]);
                if((array->ar[count] != sample->ar[rng->low]) &&
                        (array->ar[count] != sample->ar[rng->high]))
                    push(buckets[rng->high], array->ar[count]);
            }
        }
#ifdef OMP
    }
#endif
    for(count=0; count <= sample->len; count++) {
        fprintf(stdout, "Bucket %lu\n", count);
        display_stack(buckets[count]);
    }

    int_array_t* intervals = int_array_create(sample->len + 1);
#ifdef OMP
#pragma omp parallel private(count) shared(buckets, intervals)
    {
#pragma omp for schedule(static)
#endif
        for(count=0; count <= sample->len; count++)
            int_array_set(intervals, stack_size(buckets[count]), count);
#ifdef OMP
    }
#endif
    /*fprintf(stdout, "intervals\n");
    print(intervals);*/
    int_array_t* pps = parallel_prefix_sum(intervals);
    int_array_destroy(&intervals);
    /*fprintf(stdout, "PPS\n");
    print(pps);*/
    size_t s, e, s_id;
    int s_val;
#ifdef OMP
#pragma omp parallel private(count, s, e, s_id, s_val) shared(buckets, array, pps, sample_len)
    {
#pragma omp for schedule(static)
#endif
        for(count=0; count<=sample->len; count++) {
            if(count == 0) {
                s = 0;
                e = stack_size(buckets[0]);
            } else {
                s = int_array_get(pps, count-1) + count;
                e = s + stack_size(buckets[count]);
            }
            copy_from_stack(array, s, e, buckets[count]);
            destroy_stack(&buckets[count]);
            fprintf(stdout, "[%d] bucket id : %lu start : %lu, end : %lu\n", omp_get_thread_num(), count, s, e);
            if(count != sample_len) {
                //s_id = count + int_array_get(pps, count);
                s_val = int_array_get(sample, count);
                int_array_set(array, s_val, e);
            }
            parallel_partition_sort(array, s, e);
        }
#ifdef OMP
    }
#endif
    int_array_destroy(&sample);
    int_array_destroy(&pps);
}

/*
static void parallel_partition_sort(int_array_t* array, int_array_t* sample,
        size_t start, size_t end, size_t sample_index)
{
    if(debug) fprintf(stdout, "start :%lu, end:%lu, sample_index:%lu\n", start, end, sample_index);
    if(!array || !sample || end <= start || end-start == 1
            || sample_index > sample->len)
        return;
    if(sample_index == sample->len) {
        size_t sample_size = ceil(sqrt(end-start));
        int_array_t* new_sample = get_random_sample(array, start, end, sample_size);
#ifdef OMP
#pragma omp task
#endif
        parallel_partition_sort(array, new_sample, start, end, 0);
        return;
    }
    if(!sample->is_sorted) {
        qsort(sample->ar, sample->len, sizeof(int), int_comp);
        sample->is_sorted = true;
        if(debug) {
            fprintf(stdout, "sorted sample\n");
            print(sample);
        }
    }
    size_t i, new_len = 0, mask_index = 0, index, cur_index;
    int t, j;
        //new_len = 0;
        int_array_t* mask = int_array_create(end - start);
#ifdef OMP
#pragma omp parallel for  private(i, t) \
        shared(sample_index, mask, array, sample, new_len, start, end) \
        schedule(static)
#endif
        for(i=start; i<end; i++)
        {
            t = int_array_get(array, i);
            if(sample_index == 0) {
                if(t < sample->ar[0]) {
                    int_array_set(mask, 1, i-start);
#ifdef OMP
#pragma omp atomic
#endif
                    new_len++;
                } else {
                    int_array_set(mask, 0, i-start);
                }
            } else {
                if(t < sample->ar[sample_index] && t >= sample->ar[sample_index - 1]) {
                    int_array_set(mask, 1, i-start);
#ifdef OMP
#pragma omp atomic
#endif
                    new_len++;
                } else {
                    int_array_set(mask, 0, i-start);
                }
            }
        }
        int_array_t* pps = parallel_prefix_sum(mask);
        //int_array_t* new_array = int_array_create(new_len);
#ifdef DOMP
#pragma omp parallel for private(i, index) \
        shared(array, mask, pps, start, end, cur_index) schedule(static)
#endif
        for(i=start; i<end; i++)
        {
            if(1 == int_array_get(mask, i-start)) {
                index = start + int_array_get(pps, i-start) - 1;
#ifdef DOMP
#pragma omp critical
#endif
                int_swap(&(array->ar[i]), &(array->ar[index]));
            }
            if(sample->ar[sample_index] == int_array_get(array,i))
                cur_index = i;
        }
        int_swap(&(array->ar[start + new_len]), &(array->ar[cur_index]));
        if(debug) {
            fprintf(stdout,"new_len : %lu\n", new_len);
            fprintf(stdout, "MASK\n");
            print(mask);
            fprintf(stdout, "Parallel_prefix_sum \n");
            print(pps);
            fprintf(stdout, "Array \n");
            print(array);
        }
        size_t sample_size = ceil(sqrt(new_len));
        int_array_t* new_sample = NULL;
        if(new_len != 0) {
            new_sample = get_random_sample(array, start, start + new_len, sample_size);
            if(debug) {
                fprintf(stdout, "New sample\n");
                print(new_sample);
            }
        }

#ifdef OMP
#pragma omp task
#endif
        if(new_len != 0)
            parallel_partition_sort(array, new_sample, start, start + new_len, 0);
#ifdef OMP
#pragma omp task
#endif
        parallel_partition_sort(array, sample, start + new_len + 1, end, sample_index+1);
        int_array_destroy(&new_sample);
        int_array_destroy(&mask);
        int_array_destroy(&pps);
}*/

void run_parallel_partition_sort(size_t size)
{
    size_t resv_size;
    create(size);
    //resv_size = ceil(sqrt(size));
    set_with_rand(array);
    if(debug) {
        fprintf(stdout, "Original Array\n");
        print(array);
    }
    //int_array_t* reservoir = get_random_sample(array, 0, array->len, resv_size);
    //fprintf(stdout, "Reservoir Array\n");
    //print(reservoir);
#ifdef DOMP
#pragma omp parallel
    {
#endif
        //parallel_partition_sort(array, reservoir, 0, array->len, 0);
        parallel_partition_sort(array, 0, array->len/*, reservoir*/);
#ifdef DOMP
    }
#endif
    if(debug) {
        fprintf(stdout, "Sorted Array\n");
        print(array);
    }
    //int_array_destroy(&reservoir);
    destroy();
}
