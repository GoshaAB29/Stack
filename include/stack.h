#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack_settings.h"

struct Stack {
    stack_t* data = (stack_t*)ERROR_PTR;
    size_t size = 0;
    unsigned int capacity = -1;
};

enum ERRORS {
    STACK_DATA_NULLPTR = 11,
    STACK_CAPACITY_LESS_1 = 12,
    STACK_SIZE_LESS_0 = 13,
    STACK_ALREADY_DESTRUCTED = 14,


    CANT_ALLOC_MEMORY = 21,

    STACK_POP_ADDRESS_VALUE_WRONG = 31,

};

int Stac_Ctor (Stack *stk, unsigned int capacity);

int Stack_Push (Stack *stk, stack_t value);
int Stack_Pop (Stack *stk, stack_t *value);

int Stack_Resize (Stack *stk);

int Stack_Dtor (Stack *stk);
