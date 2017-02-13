#include<stdio.h>
#include"array.h"

array_impl(int);

int main(int argc, char* argv[])
{
    int_array_t* array = int_array_create(10);
    int a[] = {1,2,3,4,5,6,7,8,9,10};
    unsigned long i;
    for(i=0; i<10; i++)
        int_array_set(array, &a[i], i);
    int *x;
    for(i=0;i<10;i++)
    {
        x = int_array_get(array,i);
        fprintf(stdout, "%d ", *x);
    }
    fprintf(stdout, "\nLen : %lu\n", int_array_len(array));
    int_array_destroy(&array);
    return 0;
}
