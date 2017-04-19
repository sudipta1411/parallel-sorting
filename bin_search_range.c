#include<stdlib.h>
#include"bin_search_range.h"

range_t* create_range_struct()
{
    range_t* range = (range_t*)malloc(sizeof(range_t));
    if(!range) {
        fprintf(stderr, "Range struct mem error\n");
        return NULL;
    }
    range->low = range->high = 0l;
    return range;
}

void destroy_range_struct(range_t** range)
{
    if(range && *range) {
        free(*range);
        *range = NULL;
    }
}

/*NOTE : the array is sorted*/
range_t* bin_search_range(int* array, size_t len, int key)
{
    unsigned long low = 0, high = len - 1;
    unsigned long mid;
    range_t* rng = NULL;

    while(high - low > 1) {
        mid = low + (high - low) / 2;
        if(array[mid] == key) {
            low = high = mid;
            break;
        }
        if(array[mid] < key)
            low = mid;
        else
            high = mid;
    }
    if (key == array[0])
        low = high = 0l;
    else if(key == array[len - 1])
        low = high = len - 1;

    rng = create_range_struct();
    if(rng) {
        rng->low = low;
        rng->high = high;
    }
    return rng;
}
