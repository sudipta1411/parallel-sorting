#include<stdio.h>
#include<omp.h>
//#include"array.h"
#include"merge_sort.h"
#include"partition_sort.h"

int main(int argc, char* argv[])
{
    omp_set_num_threads(NUM_THREAD);
    //run_parallel_merge_sort(NUM);
    run_parallel_partition_sort(NUM);
    return 0;
}
