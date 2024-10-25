#include <stdio.h>

#include "STAAAk.h"

int main()
{
    const char * STAAAk_name = "stk";

    MySTAAAk stk = {};

    int capacity = 2;

    STAAAkCtor(&stk,STAAAk_name, capacity);

    STAAAkPush(&stk, 100);

    STAAAkPush(&stk, 200);

    STAAAkPush(&stk, 300);

    STAAAkType Var = 0;

    STAAAkPop(&stk, &Var);

    STAAAkDump(&stk);

    STAAAkDtor(&stk);

    return 0;
}
