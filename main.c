#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#include"merge_sort.h"
#include"partition_sort.h"

int main(int argc, char* argv[])
{
    clock_t start, end;
    fprintf(stdout, "%d\n", argc);
    long num = atol(argv[1]);
    fprintf(stdout, "%lu\n", num);
    omp_set_num_threads(NUM_THREAD);

    start = clock();
    run_parallel_merge_sort(num);
    end = clock();
    fprintf(stdout, "----[merge_sort]Execution time for array length %lu is %f seconds----\n",
            num, ((double)(end-start))/CLOCKS_PER_SEC);

    start = clock();
    run_parallel_partition_sort(num);
    end = clock();
    fprintf(stdout, "----[partition_sort]Execution time for array length %lu is %f seconds----\n",
            num, ((double)(end-start))/CLOCKS_PER_SEC);
    return 0;
}
