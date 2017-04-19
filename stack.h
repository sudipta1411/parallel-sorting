#ifndef _STACK_H
#define _STACK_H

#define MAX_STACK_SIZE 2000L

typedef struct
{
    int ar[MAX_STACK_SIZE];
    int top;
} stack_t;

stack_t* create_stack();
void destroy_stack(stack_t** stack);
void push(stack_t* stack, int elem);
int pop(stack_t* stack);

#endif /*_STACK_H*/
