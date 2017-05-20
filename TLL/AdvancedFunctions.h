#ifndef G_ADVANCEDFUNCTIONS_H
#define G_ADVANCEDFUNCTIONS_H

/*
Generated on: 2013-11-30-18.45.06

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include "TemplateArray.h"
#include "TemplateList.h"

namespace TL
{

template<typename TemplateItem>
const bool TemplateListToTemplateArray(TemplateArray<TemplateItem> &Array, const TemplateList<TemplateItem> &List)
{
    TemplateArray<TemplateItem> Temp;
    if(List.IsEmpty())
    {
        Array.TransferFrom(Temp);
        return true;
    }

    ERRORFUNCTION(TemplateListToTemplateArray,!Temp.SetSize(List.GetSize()),RETURN_BOOL)

    TemplateIterator<TemplateItem> Temp2 = List;
    Temp2.CurrToStart();
    do
    {
        Temp[Temp2.GetIter()-1] = Temp2.GetCurrItem();
    }while(Temp2.CurrNext());
    Array.TransferFrom(Temp);
    return true;
}

template<typename TemplateItem>
const bool TemplateArrayToTemplateList(TemplateList<TemplateItem> &List, const TemplateArray<TemplateItem> &Array)
{
    TemplateList<TemplateItem> Temp;
    if(Array.IsEmpty())
    {
        List.TransferFrom(Temp);
        return true;
    }

    SIZE_TYPE Iter = 0;

    while(Iter < Array.GetSize())
    {
        ERRORFUNCTION(TemplateArrayToTemplateList,!Temp.AddNodeEndNext(Array[Iter]),RETURN_BOOL)
        Iter++;
    }
    Temp.CurrToStart();
    List.TransferFrom(Temp);
    return true;
}

};

#endif
