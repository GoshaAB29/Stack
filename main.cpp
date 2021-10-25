#include "TXLib.h"
#include "include/stack.h"

#define STACK_TEST

//TODO
// log-file
//
//

int main()
{
    Stack stk = {};
    int error = 0;
    #ifdef STACK_TEST
    unsigned int capacity = 1;

    error = Stac_Ctor (&stk, capacity);

    for (int i = 0; i < 16; i++)
        error = Stack_Push (&stk, i);

    stack_t x = 1;

    for (int i = 0; i < 22; i++) {
    error = Stack_Pop (&stk, &x);
    printf ("x = %d\n", x);
    }
    printf ("stk.capacity = %d\n", stk.capacity);
    #endif

    error = Stack_Dtor (&stk);
    printf ("error number is %d\n", error);
    error = Stack_Dtor (&stk);
    printf ("error number is %d\n", error);

    return 0;
}
