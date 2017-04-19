#ifndef _BIN_SEARCH_RANGE
#define _BIN_SEARCH_RANGE
#include<stdio.h>

typedef struct {
    unsigned long low;
    unsigned long high;
} range_t;

range_t* create_range_struct();
void destroy_range_struct(range_t** range);
range_t* bin_search_range(int* array, size_t len, int key);
#endif /*_BIN_SEARCH_RANGE*/
