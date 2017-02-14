#include<stdio.h>
#include"array.h"
#include"merge_sort.h"

/*array_impl(int);
define_fptr(int);

static int comp(int*a, int*b)
{
    if(*a > *b)
        return 1;
    else if(*a < *b)
        return -1;
    return 0;
}*/

int main(int argc, char* argv[])
{
    /*int_array_t* array = int_array_create(10);
    int a[] = {1,2,3,4,5,6,7,8,9,10};
    unsigned long i;
    for(i=0; i<10; i++)
        int_array_set(array, a[i], i);
    int x;
    comparator_int f = &comp;
    int ret = f(&a[1],&a[2]);//(*comparator_int)(&a[1],&a[2]);
    fprintf(stdout, "comparator : %d\n", ret);
    for(i=0;i<10;i++)
    {
        x = int_array_get(array,i);
        fprintf(stdout, "%d ", x);
    }
    fprintf(stdout, "\nLen : %lu\n", int_array_len(array));
    int_array_destroy(&array);
    fprintf(stdout, "%d\n", int_array_get(array,1));*/
    run_parallel_merge_sort(128);

    return 0;
}
