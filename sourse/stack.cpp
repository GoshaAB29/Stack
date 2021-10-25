#include "../include/stack.h"
#include "../include/stack_settings.h"

int Stac_Ctor (Stack *stk, unsigned int capacity)
{
    assert (stk); //assert - т.к. простой пользователь никак не сможет добиться неверного адреса стека

    if (capacity <= 0)
        return STACK_CAPACITY_LESS_1;

    stk->data = (stack_t*) calloc (capacity, sizeof(stk->data[0]));

    if (stk->data == NULL) {
        stk->data = (stack_t*)ERROR_PTR;
        return CANT_ALLOC_MEMORY;
    }

    stk->capacity = capacity;
    stk->size = 0;

    return 0;
}


int Stack_Push (Stack *stk, stack_t value)
{
    int error = 0;

    assert(stk);

    stk->data[stk->size++] = value;

    if (stk->size >= stk->capacity)
            error = Stack_Resize (stk);

    if (error != 0)
        return CANT_ALLOC_MEMORY;

    return 0;
}

int Stack_Pop (Stack *stk, stack_t *value)
{
    int error = 0;

    assert (stk);

    if (value == NULL)
        return STACK_POP_ADDRESS_VALUE_WRONG;

    if (stk->size <= 0)
        return STACK_SIZE_LESS_0;

    *value = stk->data[--stk->size];

    if ((stk->size < stk->capacity / MEMORY_REALLOCATE_DOWN) && (stk->size >= 1))
    error = Stack_Resize (stk);

    if (error != 0)
        return CANT_ALLOC_MEMORY;

    return 0;
}


int Stack_Resize (Stack *stk)
{
    int error = 0;
    assert (stk);

    if (stk->size >= stk->capacity)  {
        stk->capacity = stk->capacity * MEMORY_REALLOCATE_UP;
        error = (int)realloc (stk->data, sizeof (stk->data[0]) * stk->capacity);
    }

    if ((stk->size < stk->capacity / MEMORY_REALLOCATE_DOWN)  &&
        (stk->size >= 1)) {
        stk->capacity = stk->capacity / MEMORY_REALLOCATE_DOWN;
        error = (int)realloc (stk->data, sizeof (stk->data[0]) * stk->capacity);
    }

    if (error != 0)
        return CANT_ALLOC_MEMORY;

    return 0;
}


int Stack_Dtor (Stack *stk)
{
    assert (stk);

    if (stk->data == ERROR_PTR)
        return STACK_ALREADY_DESTRUCTED ;

    free (stk->data);
    stk->data = (stack_t*)ERROR_PTR;
    stk->capacity = -1;
    stk->size = 0;

    free (stk);
    stk = (Stack*)ERROR_PTR;

    return 0;
}
