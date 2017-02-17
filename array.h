#ifndef _ARRAY_H
#define _ARRAY_H
#include<stdlib.h>
#include<stddef.h>
#include<stdio.h>

#define ARR_SIZE(x) ((sizeof x) / (sizeof *x))

/*Generic array*/
#define array_impl(type) \
    typedef struct { \
        type *ar; \
        size_t len; \
    } type##_array_t; \
\
static type##_array_t* type##_array_create(size_t len) \
{\
    type##_array_t* array = NULL; \
    array = (type##_array_t*)malloc(sizeof(type##_array_t)); \
    if(!array) return NULL; \
    array->len = len; \
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
    if(!array || index >= array->len || index < 0) return; \
    array->ar[index] = val; \
}\
\
static type type##_array_get(type##_array_t* array, unsigned long index) \
{\
    if(!array || index >= array->len || index < 0) \
    {\
        fprintf(stdout,"INDEX OUT OF BOUND\n"); \
        return NULL; \
    }\
    return array->ar[index]; \
}\
\
static size_t type##_array_len(type##_array_t* array) \
{\
    if(!array) return 0; \
    return array->len; \
}

#define ARRAY_DEFINED

#endif /*_ARRAY_H*/
