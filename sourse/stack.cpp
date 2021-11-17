#include "../include/stack.h"
#include "../include/stack_settings.h"

int Canary_Check (const Stack *stk)
{
    if (stk->canary_left != 0xC0FFEE)
        return STACK_L_CANARY_RUINED;

    if   (stk->canary_right != 0xC0FFEE)
        return STACK_R_CANARY_RUINED;


    if (!stk->canary_data_left)
        return STACK_DATA_L_CANARY_RUINED;

    if (!stk->canary_data_right)
        return STACK_DATA_R_CANARY_RUINED;

    if (*stk->canary_data_left != 0xC0FFEE)
        return STACK_DATA_L_CANARY_RUINED
        ;
    if (*stk->canary_data_right != 0xC0FFEE)
        return STACK_DATA_R_CANARY_RUINED;

    return STACK_IS_OK;
}

int Stack_Verify (Stack *stk)
{

    if ((stk == nullptr) || (stk == (Stack*)ERROR_PTR))
        return STACK_IS_NULLPTR;

    if (stk->logfile == nullptr)
        return LOGFILE_D_N_EXIST;

    if (stk->hash != 0)
        if (stk->hash != Upd_Hash (stk))
            return STACK_HASH_RUINED;

    if (isdigit (stk->capacity))
        return STACK_CAPACITY_IS_NAN;

    if (stk->capacity < 1)
        return STACK_CAPACITY_LESS_1;

    if (stk->size < 0)
        return STACK_UNDERFLOW;

    if (stk->size > stk->capacity)
        return STACK_OVERFLOW;

    if ((stk->data == nullptr) || (stk->data == ERROR_PTR))
        return STACK_DATA_NULLPTR;

    switch (Canary_Check (stk)) {
        case STACK_R_CANARY_RUINED:
                return STACK_R_CANARY_RUINED;
            break;
        case STACK_L_CANARY_RUINED:
                return STACK_L_CANARY_RUINED;
            break;
        case STACK_DATA_L_CANARY_RUINED:
                return STACK_DATA_L_CANARY_RUINED;
            break;
        case STACK_DATA_R_CANARY_RUINED:
                return STACK_DATA_R_CANARY_RUINED;
            break;
        default:
            break;
    }

    return STACK_IS_OK;
}

/*
    STACK_IS_OK = 0,                     +
    STACK_IS_NULLPTR = 1,                +
    LOGFILE_D_N_EXIST = 2,               +

    STACK_DATA_NULLPTR = 11,             +
    STACK_CAPACITY_LESS_1 = 12,          +
    STACK_CAPACITY_IS_NAN = 13,          +
    STACK_UNDERFLOW = 14,                +
    STACK_OVERFLOW = 15,                 +

    STACK_ALREADY_CTOR = 24,
    STACK_ALREADY_DTOR = 25,             +

    CANT_ALLOC_MEMORY = 31,              +

    STACK_POP_ADDRESS_VALUE_WRONG = 41,  +

    STACK_L_CANARY_RUINED = 51,          +
    STACK_R_CANARY_RUINED = 52,          +
    STACK_DATA_L_CANARY_RUINED = 53,     +
    STACK_DATA_R_CANARY_RUINED = 54,     +
    STACK_HASH_RUINED = 55,              +
*/


void Stack_Dump (Stack *stk, FILE* logfile, int error, const char* file,
                 const int line, const char* func)
{
    if (logfile == nullptr)
        logfile = fopen ("log.txt", "a+b");


    fprintf (logfile, "------------------------------------------------\n");

    if (error != STACK_IS_OK) {
        fprintf (logfile, "ERROR: %d - ", error);
        Error_Code_To_Defintion (logfile, error);
    } else
        fprintf (logfile, "STACK is OK\n");


    fprintf (logfile, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");

    fprintf (logfile, "Time: %s, %s\n", __DATE__, __TIME__);
    fprintf (logfile, "Filename is %s, line of program is %d, "
                     "function is %s\n", file, line, func);

    fprintf (logfile, ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");

    if (error == STACK_IS_NULLPTR) {
        FILE* logfile = fopen ("log.txt", "a+b");
        fprintf (logfile, "------------------------------------------------\n\n");
        fclose (logfile);
        return;
    }

    fprintf (logfile, "Stack[%p]\n", stk);


    fprintf (logfile, "{\n");


    fprintf (logfile, "    canary left = 0x%X,", stk->canary_left);
    fprintf (logfile, " canary right = 0x%X \n", stk->canary_right);

    fprintf (logfile, "    capacity = %zd\n", stk->capacity);
    fprintf (logfile, "    size = %zd\n", stk->size);

    if ((error == STACK_DATA_NULLPTR) || (error == STACK_ALREADY_DTOR)) {
        fprintf (logfile, "------------------------------------------------\n");
        return;
    }

    fprintf (logfile, "    data [%p]\n", stk->data);

    fprintf (logfile, "    {");

    fprintf (logfile, "\n");

    if ((!stk->canary_data_left == 0) || (stk->canary_data_left !=
                                        (unsigned long long*)ERROR_PTR));
        fprintf (logfile, "    left data canary = 0x%X\n", *stk->canary_data_left);


    int border = DUMP_MAX_OUTPUT_NUMBER;
    if (border > stk->capacity)
        border = stk->capacity;

    for (int i = 0; i < border; i++) {
        if (i == stk->size)
            fprintf (logfile, "----end-of-stack-size----\n");
        fprintf (logfile, "    stack->data[%d] = "STACK_FMT"\n", i, stk->data[i]);
    }

    if (border < stk->capacity)
        fprintf (logfile, "    ...\n");

    fprintf (logfile, "----end-of-capacity----\n");
    fprintf (logfile, "    right data canary = 0x%X\n",
             *stk->canary_data_right);

    fprintf (logfile, "    }\n");
    fprintf (logfile, "}\n");

    fprintf(logfile, "------------------------------------------------\n");
}

int Stack_Ctor (Stack *stk, unsigned int capacity)
{
    stk->logfile = fopen ("log.txt", "w"); // regime "a+b"

    int error = Ctor_Verify (stk, capacity);
    if (error != STACK_IS_OK) {
        STACK_DUMP (stk, error);
        return error;
    }

    size_t size_data = (2 * sizeof (*(stk->canary_data_left)) +
                    capacity * sizeof (stack_t));

    stk->canary_data_left = (unsigned long long*)calloc (size_data, sizeof (char));

    if (stk->canary_data_left == NULL) {
        stk->canary_data_left = (unsigned long long*)ERROR_PTR;
        STACK_DUMP (stk, CANT_ALLOC_MEMORY);
        return CANT_ALLOC_MEMORY;
    }

    stk->data = (stack_t*)(stk->canary_data_left + 1);
    stk->canary_data_right = (unsigned long long*)(stk->data + capacity);

    *(stk->canary_data_left) = (unsigned long long)0xC0FFEE;
    *(stk->canary_data_right) = (unsigned long long)0xC0FFEE;

    stk->capacity = capacity;
    stk->size = 0;

    stk->hash = Upd_Hash (stk);

    STACK_CHECK (stk);
    return STACK_IS_OK;
}


int Stack_Push (Stack *stk, stack_t value)
{
    int error = STACK_IS_OK;
    STACK_CHECK (stk);

    stk->data[stk->size] = value;
    stk->size++;

    if (stk->size == stk->capacity)
        error = Stack_Resize (stk);
    if (error != STACK_IS_OK)
        return error;

    stk->hash = Upd_Hash (stk);

    STACK_CHECK (stk);
    return STACK_IS_OK;
}

int Stack_Pop (Stack *stk, stack_t *value)
{
    stk->size--;

    int error = Stack_Verify (stk);

    if (error != STACK_IS_OK) {

        Stack_Dump (stk, stk->logfile, error,
        __FILE__, __LINE__, __PRETTY_FUNCTION__);
        stk->size++;
        return error;
    }

    if (value == NULL) {
        error = STACK_POP_ADDRESS_VALUE_WRONG;
        stk->size++;
        STACK_DUMP (stk, error);
        return error;
    }

    *value = stk->data[stk->size];
    stk->data[stk->size] = 0;

    if ((stk->size < stk->capacity / MEMORY_REALLOCATE_DOWN) && (stk->size >= 1))
        error = Stack_Resize (stk);

    if (error != STACK_IS_OK) {
        STACK_DUMP (stk, error);
        return error;
    }

    stk->hash = Upd_Hash (stk);
    STACK_CHECK (stk);
    return STACK_IS_OK;
}


int Stack_Resize (Stack *stk)
{
    STACK_CHECK (stk);

    if (stk->size == stk->capacity)  {
        stk->capacity = stk->capacity * MEMORY_REALLOCATE_UP;
        *stk->canary_data_right = 0;

        size_t new_size_data = (2 * sizeof (*(stk->canary_data_left)) +
                    stk->capacity * sizeof (stack_t));

        stk->canary_data_left = (unsigned long long*)realloc (stk->canary_data_left,
                                 (sizeof (char) * new_size_data));

    }

    if ((stk->size < stk->capacity / MEMORY_REALLOCATE_DOWN)  &&
        (stk->size >= 1)) {
        stk->capacity = stk->capacity / MEMORY_REALLOCATE_DOWN;
        *stk->canary_data_right = 0;

        size_t new_size_data = 2 * sizeof (stk->canary_data_left) +
                               stk->capacity * sizeof (stack_t);

        stk->canary_data_left = (unsigned long long*)realloc (stk->canary_data_left,
                                 (sizeof (char) * new_size_data));
    }

    if (!stk->canary_data_left) {
        int error = CANT_ALLOC_MEMORY;
        STACK_DUMP (stk, error);
        return error;
    }

    stk->data = (stack_t*)(stk->canary_data_left + 1);
    stk->canary_data_right = (unsigned long long*)(stk->data + stk->capacity);

    *(stk->canary_data_left) = (unsigned long long)0xC0FFEE;
    *(stk->canary_data_right) = (unsigned long long)0xC0FFEE;

    *stk->canary_data_right = 0xC0FFEE;

    stk->hash = Upd_Hash (stk);

    if (stk->hash == -1)
        return STACK_HASH_RUINED;
    STACK_CHECK (stk);

    return STACK_IS_OK;
}

size_t Upd_Hash (Stack *stk)
{
    size_t hash = 0;

    if ((stk == nullptr) || (stk == (Stack*)ERROR_PTR)) {
        FILE* logfile = fopen ("log.txt", "a+b");
        Stack_Dump (stk, logfile, STACK_IS_NULLPTR,__FILE__,
                    __LINE__, __PRETTY_FUNCTION__);
        fclose (logfile);
        return STACK_IS_NULLPTR;
    }

    stk->hash = hash;

    if (Stack_Verify (stk) != STACK_IS_OK)
        return STACK_HASH_RUINED;


    for (stack_t i = 0; i <= stk->capacity; i++)
        hash = (2 * hash + abs((size_t)stk->data[i])) % 101 >> 2;

    hash += (size_t)(stk->canary_left + stk->data + stk->size + stk->capacity
          + stk->canary_right);

    hash %= 101;

    return hash;
}

int Stack_Dtor (Stack *stk)
{
    int error = Dtor_Verify (stk);

    if (error != 0) {
            STACK_DUMP (stk, error);
            return error;
    }

    fclose (stk->logfile);
    stk->logfile = 0;

    free (stk->data);
    free (stk->canary_data_left);
    free (stk->canary_data_right);

    stk->data = (stack_t*)ERROR_PTR;
    stk->canary_data_left = (unsigned long long*)ERROR_PTR;
    stk->canary_data_right = (unsigned long long*)ERROR_PTR;

    stk->capacity = -1;
    stk->size = 0;
    stk->hash = -1;

    stk->canary_left = 0;
    stk->canary_right = 0;

    stk = (Stack*)ERROR_PTR;

    return 0;
}

int Dtor_Verify (Stack *stk)
{
    if (!stk)
        return STACK_IS_NULLPTR;

    if (stk == (Stack*)ERROR_PTR)
        return STACK_ALREADY_DTOR;

    if ((stk->data == ERROR_PTR) || (!stk->data))
        return STACK_ALREADY_DTOR;

    return STACK_IS_OK;
}

int Ctor_Verify (Stack *stk, unsigned int capacity)
{
    if (!stk)
        return STACK_IS_NULLPTR;

    if (!(capacity != NAN))
        return STACK_CAPACITY_IS_NAN;

    if (capacity <= 0)
        return STACK_CAPACITY_LESS_1;

    if (stk->data != (stack_t*)ERROR_PTR)
        return STACK_ALREADY_CTOR;

    return STACK_IS_OK;
}

void Error_Code_To_Defintion (FILE* logfile, int error)
{
    if (logfile == nullptr)
        logfile = fopen ("log.txt", "a+b");

    switch (error) {
        case STACK_IS_OK:
            fprintf (logfile, "STACK_IS_OK\n");
            break;
        case STACK_IS_NULLPTR:
            fprintf (logfile, "STACK_IS_NULLPTR\n");
            break;
        case LOGFILE_D_N_EXIST:
            fprintf (logfile, "LOGFILE_D_N_EXIST\n");
            break;
        case STACK_DATA_NULLPTR:
            fprintf (logfile, "STACK_DATA_NULLPTR\n");
            break;
        case STACK_CAPACITY_LESS_1:
            fprintf (logfile, "STACK_CAPACITY_LESS_1\n");
            break;
        case STACK_CAPACITY_IS_NAN:
            fprintf (logfile, "STACK_CAPACITY_IS_NAN\n");
            break;
        case STACK_UNDERFLOW:
            fprintf (logfile, "STACK_UNDERFLOW\n");
            break;
        case STACK_OVERFLOW:
            fprintf (logfile, "STACK_OVERFLOW\n");
            break;
        case STACK_ALREADY_CTOR:
            fprintf (logfile, "STACK_ALREADY_CTOR\n");
            break;
        case STACK_ALREADY_DTOR:
            fprintf (logfile, "STACK_ALREADY_DTOR\n");
            break;
        case CANT_ALLOC_MEMORY:
            fprintf (logfile, "CANT_ALLOC_MEMORY\n");
            break;
        case STACK_POP_ADDRESS_VALUE_WRONG:
            fprintf (logfile, "STACK_POP_ADDRESS_VALUE_WRONG\n");
            break;
                case STACK_L_CANARY_RUINED:
            fprintf (logfile, "STACK_L_CANARY_RUINED\n");
            break;
        case STACK_R_CANARY_RUINED:
            fprintf (logfile, "STACK_R_CANARY_RUINED\n");
            break;
        case STACK_DATA_L_CANARY_RUINED:
            fprintf (logfile, "STACK_DATA_L_CANARY_RUINED\n");
            break;
        case STACK_DATA_R_CANARY_RUINED:
            fprintf (logfile, "STACK_DATA_R_CANARY_RUINED\n");
            break;
        case STACK_HASH_RUINED:
            fprintf (logfile, "STACK_HASH_RUINED\n");
            break;
        default:
            fprintf (logfile, "UNKNOWN_ERROR\n");
    }
}
