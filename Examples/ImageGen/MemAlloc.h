#ifndef G_MEM_ALLOC_A_H
#define G_MEM_ALLOC_A_H

#include <stdio.h>
#include <stdlib.h>
#include <new>
#include <errno.h>
#include "ErrorMessage.h"

template<typename T>
const bool CREATEA(T * &Item, const size_t Size = 1, const int ClearInt = 0)
{
    ERRORFUNCTION(CREATEA,(Size < 1),RETURN_BOOL);
    ERRORFUNCTION(CREATEA,Item != NULL,RETURN_BOOL);

    Item = (T *)malloc(((sizeof(T))*Size));

    ERRORFUNCTION(CREATEA,Item == NULL,RETURN_BOOL);

    memset(Item,ClearInt,((sizeof(T))*Size));
    return true;
}

template<typename T>
const bool REALLOCA(T * &Item, const size_t Size = 1, const int ClearInt = 0)
{
    ERRORFUNCTION(REALLOCA,((Size == 0) && (Item == NULL)),RETURN_BOOL);

    T *Temp = NULL;
    Temp = (T *)realloc(Item,((sizeof(T))*Size));

    ERRORFUNCTION(REALLOCA,((Size != 0) && (Temp == NULL)),RETURN_BOOL);

    Item = Temp;
    Temp = NULL;
    if(Size != 0){ memset(Item,ClearInt,((sizeof(T))*Size)); }
    return true;
}

template<typename T>
const bool DELETEA(T * &Item)
{
    ERRORFUNCTION(DELETEA,Item == NULL,RETURN_BOOL);
    free(Item);

    Item = NULL;
    return true;
}

template<typename T>
const bool CREATEB(T * &Item, const size_t Size = 1)
{
    ERRORFUNCTION(CREATEB,Size < 1,RETURN_BOOL);
    ERRORFUNCTION(CREATEB,Item != NULL,RETURN_BOOL);

    Item = new (std::nothrow) T[Size];

    ERRORFUNCTION(CREATEB,Item == NULL,RETURN_BOOL);

    return true;
}

template<typename T>
const bool DELETEB(T * &Item)
{
    ERRORFUNCTION(DELETEB,Item == NULL,RETURN_BOOL);

    delete [] Item;

    Item = NULL;
    return true;
}

#endif
