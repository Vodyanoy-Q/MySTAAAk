#include <stdio.h>
#include <stdlib.h>

#include "STAAAk.h"

int STAAAkCtor(struct MySTAAAk * stk,const char * STAAAk_name, size_t capacity)
{
    MY_ASSERT(stk);

    stk->data = (STAAAkType*)calloc(capacity + 2, sizeof(STAAAkType));
    MY_ASSERT(stk->data);
    stk->data = stk->data + 1;

    stk->canary_1 = CANARY_VAL_1;

    stk->STAAAk_name = STAAAk_name;

    stk->capacity = capacity;
    stk->size = 0;

    PoisonFiller(stk, 0);
    *(stk->data - 1) = DATA_CANARY_VAL_1;
    stk->data[stk->capacity] = DATA_CANARY_VAL_2;

    stk->hash = Djb2_hash(stk);

    stk->canary_2 = CANARY_VAL_2;

    return 0;
}

int STAAAkPush(struct MySTAAAk * stk, STAAAkType value)
{
    STAAAkVerify(stk);

    if (stk->size == stk->capacity)
    {
        STAAAkChange(stk, UP);
    }

    stk->data[stk->size] = value;

    stk->size += 1;

    stk->hash = Djb2_hash(stk);

    return 0;
}

void STAAAkChange(struct MySTAAAk * stk, STAAAK_CHANGE condition)
{
    switch (condition)
    {
        case UP:
        {
            stk->data[stk->capacity] = POISON;

            stk->capacity *= 2;

            stk->data = (STAAAkType*)realloc(stk->data - 1, (stk->capacity + 2) * sizeof(STAAAkType)) + 1;
            MY_ASSERT(stk->data);

            stk->data[stk->capacity] = DATA_CANARY_VAL_2;

            PoisonFiller(stk, stk->size);

            break;
        }

        case DOWN:
        {
            stk->data[stk->capacity] = POISON;

            if (stk->size == 0)
            {
                stk->capacity = 1;
            }
            else
            {
                stk->capacity /= 4;
            }

            stk->data = (STAAAkType*)realloc(stk->data - 1, (stk->capacity + 2) * sizeof(STAAAkType)) + 1;
            MY_ASSERT(stk->data);

            stk->data[stk->capacity] = DATA_CANARY_VAL_2;

            break;
        }

        default:
        {
            printf("ERROR STAAAk CHANGE VALUE\n");
            exit(CHANGE_ERROR);
            break;
        }
    }
}

void PoisonFiller(struct MySTAAAk * stk, size_t poison_counter)
{
    for (;poison_counter < stk->capacity; poison_counter++)
    {
        stk->data[poison_counter] = POISON;
    }

    printf("\n");
}

int STAAAkPop(struct MySTAAAk * stk, STAAAkType * var)
{
    STAAAkVerify(stk);

    if (stk->size == 0)
    {
        printf("There is no values in the STAAAk. Push values in the STAAAk and try again\n");
    }
    else
    {
        *var = stk->data[stk->size - 1];

        stk->data[stk->size - 1] = POISON;

        stk->size -= 1;

        if (stk->size == 0)
        {
            stk->capacity = 1;

            STAAAkChange(stk, DOWN);
        }

        else if (stk->size > 0 && stk->capacity / 4 == stk->size)
        {
            STAAAkChange(stk, DOWN);
        }
    }

    stk->hash = Djb2_hash(stk);

    return 0;
}

int STAAAkDtor(struct MySTAAAk * stk)
{
    free(stk->data);

    stk->canary_1 = 0;
    stk->STAAAk_name = NULL;
    stk->data = NULL;
    stk->size = 0;
    stk->hash = 0;
    stk->capacity = 0;
    stk->canary_2 = 0;

    return 0;
}

int STAAAkVerify(struct MySTAAAk * stk)
{
    if (stk == NULL)
    {
        printf("%sSTK ADDRES IS NOT OKEY%s\n\n", RED, RESET_COLOR);

        STAAAkDump(stk);

        exit (STK_ADDRES_ERROR);
    }

    if (stk->canary_1 != CANARY_VAL_1 ||
        stk->canary_2 != CANARY_VAL_2)
    {
        printf("%sCANARY VALUE IS NOT OKEY%s\n\n", RED, RESET_COLOR);

        STAAAkDump(stk);

        exit (CANARY_ERROR);
    }

    if (stk->data == NULL)
    {
        printf("%sDATA ADDRES IS NOT OKEY%s\n\n", RED, RESET_COLOR);

        STAAAkDump(stk);

        exit (DATA_ERROR);
    }

    if (*(stk->data - 1)         != DATA_CANARY_VAL_1 ||
        stk->data[stk->capacity] != DATA_CANARY_VAL_2)
    {
        printf("%sDATA CANARY VALUE IS NOT OKEY%s\n", RED, RESET_COLOR);

        STAAAkDump(stk);

        exit (DATA_CANARY_ERROR);
    }

    if (stk->hash != Djb2_hash(stk))
    {
        printf("%sHASH IS NOT OKEY%s\n", RED, RESET_COLOR);

        STAAAkDump(stk);

        exit (HASH_ERROR);
    }

    return 0;
}

void STAAAkDump(struct MySTAAAk * stk)
{
    FILE * LOG = fopen("LOG.txt", "w");

    if (LOG != NULL)
    {

        fprintf(LOG,"=======================================\n");

        if (stk == NULL)
        {
            fprintf(LOG,"STAAAk ADDRES is NULL"
                        "It is impossible to provide information");

            fprintf(LOG,"=======================================\n");

            fclose(LOG);

        }
        else
        {
            fprintf(LOG,"STAAAk Name = %s\n", stk->STAAAk_name);

            fprintf(LOG,"canary 1 = %d\n", stk->canary_1);
            fprintf(LOG,"canary 2 = %d\n", stk->canary_2);
            fprintf(LOG,"data = %p\n", stk->data);
            fprintf(LOG,"data canary 1 = %d\n", *(stk->data - 1));
            fprintf(LOG,"data canary 2 = %d\n", stk->data[stk->capacity]);
            fprintf(LOG,"hash = %lu\n", stk->hash);

            if(stk->data == NULL)
            {
                fprintf(LOG,"Data is NULL"
                            "Cant write STAAAk\n");
            }
            else
            {
                fprintf(LOG,"-------------------------\n");

                fprintf(LOG,"STAAAk\n\n");

                if (stk->capacity != 0)
                {
                    for (unsigned int i = 0; i < stk->capacity; i++)
                    {
                        if (i < stk->size)
                        {
                            fprintf(LOG,"*[%2d] = %d\n", i + 1, stk->data[i]);
                        }
                        else
                        {
                            fprintf(LOG," [%2d] = %d [POISON]\n", i + 1, stk->data[i]);
                        }
                    }
                }
                else if(stk->size != 0)
                {
                    fprintf(LOG,"Capacity = 0. Write data from size\n");

                    for (unsigned int i = 0; i < stk->size; i++)
                    {
                        if (i < stk->size)
                        {
                            fprintf(LOG,"*[%2d] = %d\n", i + 1, stk->data[i]);
                        }
                        else
                        {
                            fprintf(LOG," [%2d] = %d [POISON]\n", i + 1, stk->data[i]);
                        }
                    }

                }
                else
                {
                    fprintf(LOG,"Size and capacity is zero"
                                "Try to write data from sizeof");

                    unsigned int sizef = sizeof(stk->data)/(stk->data[0]);

                    for (unsigned int i = 0; i < sizef; i++)
                    {
                        fprintf(LOG,"*[%2d] = %d\n", i + 1, stk->data[i]);
                    }
                }

                fprintf(LOG,"\nEND OF STAAAk\n");

                fprintf(LOG,"-------------------------\n");
            }

            fprintf(LOG,"=======================================\n");

            fclose(LOG);
        }
    }
    else
    {
        printf("FILE CANT BE OPENED\n");

        printf("=======================================\n");

        if (stk == NULL)
        {
            printf("STAAAk ADDRES is NULL"
                        "It is impossible to provide information");

            printf("=======================================\n");

        }
        else
        {
            printf("%sSTAAAk Name%s %s=%s %s%s%s\n", BLUE, RESET_COLOR, RED, RESET_COLOR,  CYAN, stk->STAAAk_name, RESET_COLOR);

            printf("%scanary 1%s %s=%s %s%d%s\n", BLUE, RESET_COLOR, RED, RESET_COLOR,  CYAN, stk->canary_1, RESET_COLOR);
            printf("%scanary 2%s %s=%s %s%d%s\n", BLUE, RESET_COLOR, RED, RESET_COLOR,  CYAN, stk->canary_2, RESET_COLOR);
            printf("%sdata%s %s=%s %s%p%s\n", BLUE, RESET_COLOR, RED, RESET_COLOR,  CYAN, stk->data, RESET_COLOR);
            printf("%sdata canary 1%s %s=%s %s%d%s\n", BLUE, RESET_COLOR, RED, RESET_COLOR,  CYAN, *(stk->data - 1), RESET_COLOR);
            printf("%sdata canary 2%s %s=%s %s%d%s\n", BLUE, RESET_COLOR, RED, RESET_COLOR,  CYAN, stk->data[stk->capacity], RESET_COLOR);
            printf("%shash%s %s=%s %s%lu%s\n", BLUE, RESET_COLOR, RED, RESET_COLOR,  CYAN, stk->hash,RESET_COLOR);

            if(stk->data == NULL)
            {
                printf("Data is NULL"
                       "Cant write STAAAk\n");
            }
            else
            {
                printf("-------------------------\n");

                printf("STAAAk\n\n");

                if (stk->capacity != 0)
                {
                    for (unsigned int i = 0; i < stk->capacity; i++)
                    {
                        if (i < stk->size)
                        {
                            printf("*[%2d] = %d\n", i + 1, stk->data[i]);
                        }
                        else
                        {
                            printf(" [%2d] = %d [POISON]\n", i + 1, stk->data[i]);
                        }
                    }
                }
                else if(stk->size != 0)
                {
                    printf("Capacity = 0. Write data from size\n");

                    for (unsigned int i = 0; i < stk->size; i++)
                    {
                        if (i < stk->size)
                        {
                            printf("*[%2d] = %d\n", i + 1, stk->data[i]);
                        }
                        else
                        {
                            printf(" [%2d] = %d [POISON]\n", i + 1, stk->data[i]);
                        }
                    }

                }
                else
                {
                    printf("Size and capacity is zero"
                                "Try to write data from sizeof");

                    unsigned int sizef = sizeof(stk->data)/(stk->data[0]);

                    for (unsigned int i = 0; i < sizef; i++)
                    {
                        printf("*[%2d] = %d\n", i + 1, stk->data[i]);
                    }
                }

                printf("\nEND OF STAAAk\n");

                printf("-------------------------\n");
            }
        }

        printf("=======================================\n");

        fclose(LOG);

        exit (FILE_ERROR);
    }
}

unsigned int Djb2_hash(struct MySTAAAk * stk)
{
    unsigned long hash = 5381;

    for(unsigned int i = 0; i < stk-> capacity + 1; i++)
    {
        hash = ((hash << 5) + hash) + stk->data[i];
    }

    return hash;
}
