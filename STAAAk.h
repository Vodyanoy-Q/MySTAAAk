#ifndef _STAAAK_H_
#define _STAAAK_H_

#define MY_ASSERT(addres) if(!addres)                                                        \
                          {                                                                  \
                              printf(" ERROR\n line %d\n file %s\n func %s\n",               \
                                     __LINE__, __FILE__, __func__);                          \
                                                                                             \
                              STAAAkDump(stk);                                               \
                                                                                             \
                              exit (ADDRES_ERROR);                                           \
                          }                                                                  \

#define ELEMENT_VERIFY(msg, elem) printf("%s%s IS NOT OKEY%s\n\n", RED, msg, RESET_COLOR);\
                                                                                             \
                                  STAAAkDump(stk);                                           \
                                                                                             \
                                  exit (msg_##ERROR);                                        \

//==============-COLORS-=================

#define RESET_COLOR  "\033[0m"
#define RED          "\033[1;31m"
#define MAGN         "\033[1;35m"
#define WHITE        "\033[1;37m"
#define GREEN        "\033[1;32m"
#define BLUE         "\033[1;34m"
#define CYAN         "\033[1;36m"
#define YELLOW       "\033[1;33m"

//===============-TYPEDEF-===============

typedef int STAAAkType;

//================-ENUMS-================

enum ERROR_CODE
{
    NO_ERROR = 0,
    ERROR = 1,
    CANARY_ERROR = -2,
    STK_ADDRES_ERROR = -3,
    DATA_ERROR = -4,
    DATA_CANARY_ERROR = -5,
    HASH_ERROR = -6,
    CHANGE_ERROR = -7,
    ADDRES_ERROR = -8,
    FILE_ERROR = -9,
};

enum STAAAK_CHANGE
{
    UP = 1,
    DOWN = 0
};

//==============-CONSTANTS-==============

const int CANARY_VAL_1 = 0xEDA0ADE;
const int CANARY_VAL_2 = 0xDED0DED;
const STAAAkType DATA_CANARY_VAL_1 = 0x7F;
const STAAAkType DATA_CANARY_VAL_2 = -0x7F;
const STAAAkType POISON = -19;

//===============-STRUCT-================

struct MySTAAAk
{
    int canary_1 = CANARY_VAL_1;

    const char * STAAAk_name = NULL;

    STAAAkType * data = NULL;

    size_t size = 0;
    size_t capacity = 0;

    unsigned long hash = 0;

    int canary_2 = CANARY_VAL_1;
};

//==============-FUCNTIONS-==============

int STAAAkCtor(struct MySTAAAk * stk,const char * STAAAk_name, size_t capacity);
int STAAAkPush(struct MySTAAAk * stk, STAAAkType value);
int STAAAkPop(struct MySTAAAk * stk, STAAAkType * var);
void STAAAkChange(struct MySTAAAk * stk, STAAAK_CHANGE condition);
void PoisonFiller(struct MySTAAAk * stk, size_t poison_counter);
int STAAAkDtor(struct MySTAAAk * stk);
unsigned int Djb2_hash(struct MySTAAAk * stk);
int STAAAkVerify(struct MySTAAAk * stk);
void STAAAkDump(struct MySTAAAk * stk);


#endif // _STAAAK_H_
