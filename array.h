#ifndef _ARRAY_H
#define _ARRAY_H
#include<stdlib.h>
#include<stddef.h>
#include<stdio.h>
#include<stdbool.h>

#define ARR_SIZE(x) ((sizeof x) / (sizeof *x))
#define DEFAULT_ARRAY_LEN 2048L

/*Generic array*/
#define array_impl(type) \
    typedef struct { \
        type *ar; \
        size_t len; \
        /*This is the actual size of the array*/\
        size_t size; \
        unsigned long cur; \
        bool is_sorted; \
    } type##_array_t; \
\
static type##_array_t* type##_array_create(size_t len) \
{\
    type##_array_t* array = NULL; \
    array = (type##_array_t*)malloc(sizeof(type##_array_t)); \
    if(!array) return NULL; \
    array->len = len; \
    array->size = 0l; \
    array->cur = 0l; \
    array->is_sorted = false; \
    array->ar = (type*)malloc(len*sizeof(type)); \
    if(!array->ar) return NULL; \
    return array;\
}\
\
static void type##_array_destroy(type##_array_t** array) \
{\
    if(array && *array) \
    {\
        free(*array); \
        *array = NULL; \
    }\
}\
\
static void type##_array_set(type##_array_t* array, type val, unsigned long index) \
{\
    if(!array || index >= array->len || index < 0) { \
        fprintf(stderr, "[SET]ARRAY INDEX OUT OF BOUND %lu > %lu\n", index, array->len-1); \
        return; \
    } \
    array->ar[index] = val; \
    array->size = array->size + 1; \
}\
\
static type type##_array_get(type##_array_t* array, unsigned long index) \
{\
    if(!array || index >= array->len || index < 0) \
    {\
        fprintf(stdout,"[GET]INDEX OUT OF BOUND %lu > %lu\n", index, array->len-1); \
        return NULL; \
    }\
    return array->ar[index]; \
}\
\
static size_t type##_array_len(type##_array_t* array) \
{\
    if(!array) return 0; \
    return array->len; \
}\
static type##_array_t* type##_array_add(type##_array_t* array1, type##_array_t* array2) \
{\
    if(!array1 || !array2) return NULL; \
    array1->ar = (type*)realloc(array1->ar,(array1->len + array2->len) * sizeof(type)); \
    if(!array1->ar) return NULL; \
    array1->len += array2->len; \
    unsigned long i; \
    for(i=0; i< array2->len; i++) \
        array1->ar[array1->len + i] = array2->ar[i]; \
    return array1; \
}

#define ARRAY_DEFINED

#endif /*_ARRAY_H*/
