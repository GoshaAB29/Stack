#include "../include/stack_test.h"
#include "../include/stack.h"

#define STACK_TEST
//#define TEST_OUTPUT

int Test ()
{
    Stack stk = {};
    int error = STACK_IS_OK;

    unsigned int capacity = 1;
    error = Stack_Ctor (&stk, capacity);
    error = Stack_Ctor (&stk, 4);

    #ifdef STACK_TEST

    Stack_Push (&stk, 99);
    Stack_Dump (&stk, stk.logfile, 0, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    for (int i = 0; i < 10; i++)
            Stack_Push (&stk, 3-2*i);

    Stack_Dump (&stk, stk.logfile, 0, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    stack_t x = 5;
    for (int i = 0; i < 11; i++) {
        Stack_Pop (&stk, &x);
    }

    Stack_Dump (&stk, stk.logfile, 0, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    #ifdef TEST_OUTPUT
    printf ("stk.capacity = %d\n", stk.capacity);
    printf ("error number is %d\n", error);
    #endif
    #endif
    Stack_Dump (&stk, stk.logfile, 33, __FILE__, __LINE__, __PRETTY_FUNCTION__);

    Stack_Dtor (&stk);
    Stack_Dtor (&stk);

    printf ("Test completed");
    return 0;
}
