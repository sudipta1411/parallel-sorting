#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include "stack.h"

static omp_lock_t readlock;
static omp_lock_t writelock;

stack_t* create_stack()
{
    omp_init_lock(&readlock);
    omp_init_lock(&writelock);
    stack_t* stack = (stack_t*)malloc(sizeof(stack_t));
    if(!stack) return NULL;
    stack->top = -1;
    return stack;
}

void destroy_stack(stack_t** stack)
{
    if(stack && *stack) {
        free(*stack);
        *stack = NULL;
    }
    omp_destroy_lock(&readlock);
    omp_destroy_lock(&writelock);
}

void push(stack_t* stack, int elem)
{
    if(stack->top == MAX_STACK_SIZE - 1) {
        fprintf(stderr, "FULL STACK\n");
        return;
    }
    omp_set_lock(&writelock);
    stack->ar[stack->top+1] = elem;
    stack->top = stack->top + 1;
    omp_unset_lock(&writelock);
}

int pop(stack_t* stack)
{
    int ret;
    if(stack->top == -1) {
        fprintf(stderr, "EMPTY STACK\n");
        return;
    }
    omp_set_lock(&readlock);
    ret = stack->ar[stack->top];
    stack->top = stack->top - 1;
    omp_unset_lock(&readlock);
    return ret;
}

void display_stack(stack_t* stack)
{
    if(stack) {
        int top = stack->top;
        while(top >= 0) {
            fprintf(stdout, "%d ", stack->ar[top]);
            --top;
        }
        fprintf(stdout, "\n");
    }
}

int stack_size(stack_t* stack)
{
    if(stack)
        return stack->top + 1;
    return 0;
}
