#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include "stack_settings.h"


#define STACK_CHECK (stk); {                                \
    int error_stk = Stack_Verify (stk);                     \
                                                            \
    if (error_stk != STACK_IS_OK) {                         \
                                                            \
        Stack_Dump (stk, stk->logfile, error_stk,           \
        __FILE__, __LINE__, __PRETTY_FUNCTION__);           \
        return error_stk;                                   \
    }                                                       \
}


#define STACK_DUMP (stk, error); {                          \
    Stack_Dump (stk, stk->logfile, error, __FILE__,         \
                __LINE__, __PRETTY_FUNCTION__);             \
}

struct Stack {
    unsigned long long canary_left = 0xC0FFEE;
    long long size = 0;
    unsigned int capacity = -1;

    unsigned long long *canary_data_left = (unsigned long long*)ERROR_PTR;
    stack_t *data = (stack_t*)ERROR_PTR;
    unsigned long long *canary_data_right = (unsigned long long*)ERROR_PTR;

    size_t hash = 0;
    FILE *logfile =  0;
    unsigned long long canary_right = 0xC0FFEE;
};

enum ERRORS {
    STACK_IS_OK = 0,
    STACK_IS_NULLPTR = 1,
    LOGFILE_D_N_EXIST = 2,

    STACK_DATA_NULLPTR = 3,
    STACK_CAPACITY_LESS_1 = 4,
    STACK_CAPACITY_IS_NAN = 5,
    STACK_UNDERFLOW = 6,
    STACK_OVERFLOW = 7,

    STACK_ALREADY_CTOR = 8,
    STACK_ALREADY_DTOR = 9,

    CANT_ALLOC_MEMORY = 10,

    STACK_POP_ADDRESS_VALUE_WRONG = 11,

    STACK_L_CANARY_RUINED = 12,
    STACK_R_CANARY_RUINED = 13,
    STACK_DATA_L_CANARY_RUINED = 14,
    STACK_DATA_R_CANARY_RUINED = 15,
    STACK_HASH_RUINED = 16,
};

int Canary_Check (const Stack *stk);
int Stack_Verify (const Stack *stk);
void Stack_Dump (Stack *stk, FILE *logfile, int error, const char *file,
                 const int line, const char* func);

int Stack_Ctor (Stack *stk, unsigned int capacity);

int Stack_Push (Stack *stk, stack_t value);
int Stack_Pop (Stack *stk, stack_t *value);

int Stack_Resize (Stack *stk);

size_t Upd_Hash (Stack *stk);

int Stack_Dtor (Stack *stk);
int Dtor_Verify (Stack *stk);
int Ctor_Verify (Stack *stk, unsigned int capacity);

void Error_Code_To_Defintion (FILE *logfile, int error);
