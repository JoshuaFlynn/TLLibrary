#ifndef G_COMMONFUNCTIONS_H
#define G_COMMONFUNCTIONS_H

/*
Generated on: 2011-10-21-18.21.23

Version: 2.4
*/

/*
Notes:
Added uint8_t stdlib.h

Added char variant of find (finally)

Added ScanUntilNumeric

Added IsIPv4, IsIPv6, and IsArrayIPv4 functions

Cross-compiler edition

This is the ForumBot edition, of which similar functions have been moved together.
Suggest purging the duplicate functions and updating the TL to bring in-line.

MeasureBetweenPoints function added.

Added IsArray support for C

Added CountNumberOfItems
*/

/*
Suggestions for improvements:
It is my view that a number of functions here have been duplicated
As such, duplicate seeming functions will be placed together, then purged
The purged duplicate function names will be written above the function they match

Introduce a CharArray version of ExtractSubString into CharArray. Version should be named 'ExtractCharArray'.
*/

#include "ErrorMessage.h"
#include <vector>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdint.h> //uint8_t

namespace TL
{

//One of the complaints that cropped up about std::string (and perhaps applicable to other classes)
//was that it had a lot of public methods that were probably better as externalised generic functions
//In light of such commentary, generic externalised functions are supplied where possible
template<typename TemplateItem>
TemplateItem * Copy(TemplateItem Data1[], const TemplateItem Data2[], const SIZE_TYPE N)
{
    if(Data1 == NULL || Data2 == NULL){return NULL;}
    TemplateItem *Temp = (Data1+N);
    while(Data1 != Temp){*Data1 = *Data2; Data1++; Data2++;}
    return Data1;
}

template<typename TemplateItem>
TemplateItem * Copy(TemplateItem Data1[], const TemplateItem Data2[], const TemplateItem Data3[])
{
    if(Data1 == NULL || Data2 == NULL || Data3 == NULL){return NULL;}
    if(Data3 < Data2){return NULL;}
    return Copy(Data1,Data2,Data3-Data2);
}

template<typename TemplateItem>
const TemplateItem * FindItem(const TemplateItem Start[], const TemplateItem End[], const TemplateItem &Item)
{
    while(Start != End)
    {
        if(*Start == Item){return Start;}
        Start++;
    }
    return NULL;
}

template<typename TemplateItem>
TemplateItem * FindItem(TemplateItem Start[], const TemplateItem End[], const TemplateItem &Item)
{
    while(Start != End)
    {
        if(*Start == Item){return Start;}
        Start++;
    }
    return NULL;
}

template<typename TemplateItem>
TemplateItem * FindItem(TemplateItem StartList[], const SIZE_TYPE S, const TemplateItem &Item)
{
    return FindItem(StartList,StartList+S,Item);
}

template<typename TemplateItem>
const TemplateItem * FindItem(const TemplateItem StartList[], const SIZE_TYPE S, const TemplateItem &Item)
{
    return FindItem(StartList,StartList+S,Item);
}

//CompareList is believed to be a duplicate version of finditem
//CompareList seems to return boolean, FindItem does not, but FindItem should replace CompareList
template<typename TemplateItem>
const bool CompareList(const TemplateItem &Item, const TemplateItem ListStart[], const TemplateItem ListEnd[])
{
    while(ListStart != ListEnd)
    {
        if(Item == *ListStart){return true;}
    	ListStart++;
    }
    return false;
}

template<typename TemplateItem>
const bool CompareList(const TemplateItem &Item, const TemplateItem ListStart[], const SIZE_TYPE S)
{
    return CompareList(Item,ListStart,ListStart+S);
}


//Checks 'IsItem' in the array?
//Is this a possible duplicate of 'FindItem' and 'CompareList'?
template<typename TemplateItem>
const bool IsItem(const TemplateItem &Target, const TemplateItem StartItemList[], const TemplateItem EndItemList[])
{
    while(StartItemList != EndItemList)
    {
        if(Target == *StartItemList){return true;}
        ++StartItemList;
    }
    return false;
}

template<typename TemplateItem>
const bool IsItem(const TemplateItem &Target, const TemplateItem ItemList[], const SIZE_TYPE S)
{
    return IsItem(Target,ItemList,ItemList+S);
}

//

template<typename TemplateItem>
const TemplateItem * FindItemBackwards(const TemplateItem Start[], const TemplateItem End[], const TemplateItem &Item)
{
    while(End != Start)
    {
        if(*End == Item){return End;}
        End--;
    }
    return NULL;
}

template<typename TemplateItem>
TemplateItem * FindItemBackwards(const TemplateItem Start[], TemplateItem End[], const TemplateItem &Item)
{
    while(End != Start)
    {
        if(*End == Item){return End;}
        End--;
    }
    return NULL;
}

template<typename TemplateItem>
TemplateItem * FindItemBackwards(TemplateItem StartList[], const SIZE_TYPE S, const TemplateItem &Item)
{
    return FindItemBackwards(StartList,StartList+S-1,Item);
}

template<typename TemplateItem>
const TemplateItem * FindItemBackwards(const TemplateItem StartList[], const SIZE_TYPE S, const TemplateItem &Item)
{
    return FindItemBackwards(StartList,StartList+S-1,Item);
}

template<typename TemplateItem>
const TemplateItem *Find(const TemplateItem Start[], const TemplateItem End[], const TemplateItem StartSubStr[], const TemplateItem EndSubStr[], const bool ReturnLast)
{
    const TemplateItem *Temp = NULL;
    while( (Start = FindItem(Start,End,*StartSubStr)))
    {
        Temp = StartSubStr;
        while(true)
        {
            if(Temp == EndSubStr)
            {
                if(ReturnLast)
                {
                    Start--;
                    return Start;
                }
                else
                {
                    return (Start-(EndSubStr-StartSubStr));
                }
            }
            if( (*Start) != (*Temp) ){ break; }
            if(Start == End){ return NULL; }
            ++Start;
            ++Temp;
        }
    }
    return NULL;
}

template<typename TemplateItem>
const TemplateItem *Find(const TemplateItem Start[], const TemplateItem End[], const TemplateItem SubStr[], const SIZE_TYPE S, const bool ReturnLast)
{
    return Find(Start,End,SubStr,SubStr+S,ReturnLast);
}

template<typename TemplateItem>
const TemplateItem *Find(const TemplateItem Start[], const SIZE_TYPE S, const TemplateItem SubStr[], const SIZE_TYPE S2, const bool ReturnLast)
{
    return Find(Start,Start+S,SubStr,SubStr+S2,ReturnLast);
}

template<typename TemplateItem>
TemplateItem *Find(TemplateItem Start[], const TemplateItem End[], const TemplateItem StartSubStr[], const TemplateItem EndSubStr[], const bool ReturnLast)
{
    const TemplateItem *Temp = NULL;
    while( (Start = FindItem(Start,End,*StartSubStr)))
    {
        Temp = StartSubStr;
        while(true)
        {
            if(Temp == EndSubStr)
            {
                if(ReturnLast)
                {
                    Start--;
                    return Start;
                }
                else
                {
                    return (Start-(EndSubStr-StartSubStr));
                }
            }

            if( (*Start) != (*Temp) ){ break; }
            if(Temp > EndSubStr){ break; }

            if(Start == End)
            {
                return NULL;
            }
            ++Start;
            ++Temp;
        }
    }
    return NULL;
}

template<typename TemplateItem>
TemplateItem *Find(TemplateItem Start[], const TemplateItem End[], const TemplateItem SubStr[], const SIZE_TYPE S, const bool ReturnLast = true)
{
    return Find(Start,End,SubStr,SubStr+S,ReturnLast);
}

template<typename TemplateItem>
TemplateItem *Find(TemplateItem Start[], const SIZE_TYPE S, const TemplateItem SubStr[], const SIZE_TYPE S2, const bool ReturnLast = true)
{
    return Find(Start,Start+S,SubStr,SubStr+S2,ReturnLast);
}

template<typename TemplateItem>
TemplateItem *Find(TemplateItem Start[], const SIZE_TYPE End, const TemplateItem StartSubStr[], const TemplateItem EndSubStr[], const bool ReturnLast = true)
{
    return Find(Start,Start+End,StartSubStr,EndSubStr,ReturnLast);
}

const char *Find(const char ItemCopy1[], const char ItemCopy2[], const bool ReturnLast)
{
    if(!ItemCopy1 || !ItemCopy2){return NULL;}
    return Find(ItemCopy1,strlen(ItemCopy1),ItemCopy2,strlen(ItemCopy2),ReturnLast);
}

//=================
//ScanUntil Section
//=================

//It is my opinion 'ScanUntil' is a version of 'FindItem'

//ScanUntil Const Versions!

template<typename TemplateItem>
const TemplateItem * ScanUntil(const TemplateItem Start[], const TemplateItem End[], const TemplateItem &Item)
{
    while(Start != End)
    {
        if(*Start == Item){return Start;}
        Start++;
    }
    return NULL;
}

template<typename TemplateItem>
const TemplateItem * ScanUntil(const TemplateItem Start[], const SIZE_TYPE S, const TemplateItem &Item)
{
    return ScanUntil(Start,Start+S,Item);

}

template<typename TemplateItem>
const TemplateItem * ScanUntil(const TemplateItem Start[], const TemplateItem End[], const TemplateItem ItemListStart[], const TemplateItem ItemListEnd[])
{
    while(Start != End)
    {
        if(CompareList(*Start,ItemListStart,ItemListEnd))
        {
            return Start;
        }
        Start++;
    }
    return NULL;
}

template<typename TemplateItem>
const TemplateItem * ScanUntil(const TemplateItem Start[], const TemplateItem End[], const TemplateItem ItemListStart[], const SIZE_TYPE S)
{
    return ScanUntil(Start,End, ItemListStart, ItemListStart+S);
}

template<typename TemplateItem>
const TemplateItem * ScanUntil(const TemplateItem Start[], const SIZE_TYPE S, const TemplateItem ItemListStart[], const TemplateItem ItemListEnd[])
{
    return ScanUntil(Start,Start+S, ItemListStart, ItemListEnd);
}

template<typename TemplateItem>
const TemplateItem * ScanUntil(const TemplateItem Start[], const SIZE_TYPE S1, const TemplateItem ItemListStart[], const SIZE_TYPE S2)
{
    return ScanUntil(Start,Start+S1, ItemListStart, ItemListStart+S2);
}

//ScanUntil non-const versions

template<typename TemplateItem>
TemplateItem * ScanUntil(TemplateItem Start[], const TemplateItem End[], const TemplateItem &Item)
{
    while(Start != End)
    {
        if(*Start == Item){return Start;}
        Start++;
    }
    return NULL;
}

template<typename TemplateItem>
TemplateItem * ScanUntil(TemplateItem Start[], const SIZE_TYPE S, const TemplateItem &Item)
{
    return ScanUntil(Start,Start+S,Item);

}

template<typename TemplateItem>
TemplateItem * ScanUntil(TemplateItem Start[], const TemplateItem End[], const TemplateItem ItemListStart[], const TemplateItem ItemListEnd[])
{
    while(Start != End)
    {
        if(CompareList(*Start,ItemListStart,ItemListEnd))
        {
            return Start;
        }
        Start++;
    }
    return NULL;
}

template<typename TemplateItem>
TemplateItem * ScanUntil(TemplateItem Start[], const TemplateItem End[], const TemplateItem ItemListStart[], const SIZE_TYPE S)
{
    return ScanUntil(Start,End, ItemListStart, ItemListStart+S);
}

template<typename TemplateItem>
TemplateItem * ScanUntil(TemplateItem Start[], const SIZE_TYPE S, const TemplateItem ItemListStart[], const TemplateItem ItemListEnd[])
{
    return ScanUntil(Start,Start+S, ItemListStart, ItemListEnd);
}

template<typename TemplateItem>
TemplateItem * ScanUntil(TemplateItem Start[], const SIZE_TYPE S1, const TemplateItem ItemListStart[], const SIZE_TYPE S2)
{
    return ScanUntil(Start,Start+S1, ItemListStart, ItemListStart+S2);
}

//====================
//ScanUntilNot Section
//====================

//ScanUntilNot non-const section


template<typename TemplateItem>
const TemplateItem * ScanUntilNot(const TemplateItem Start[], const TemplateItem End[], const TemplateItem &Item)
{
    while(Start != End)
    {
        if(*Start != Item){return Start;}
        Start++;
    }
    return NULL;
}

template<typename TemplateItem>
const TemplateItem * ScanUntilNot(const TemplateItem Start[], const SIZE_TYPE S, const TemplateItem &Item)
{
    return ScanUntilNot(Start,Start+S,Item);

}

template<typename TemplateItem>
const TemplateItem * ScanUntilNot(const TemplateItem Start[], const TemplateItem End[], const TemplateItem ItemListStart[], const TemplateItem ItemListEnd[])
{
    while(Start != End)
    {
        if(!CompareList(*Start,ItemListStart,ItemListEnd))
        {
            return Start;
        }
        Start++;
    }
    return NULL;
}

template<typename TemplateItem>
const TemplateItem * ScanUntilNot(const TemplateItem Start[], const TemplateItem End[], const TemplateItem ItemListStart[], const SIZE_TYPE S)
{
    return ScanUntilNot(Start,End, ItemListStart, ItemListStart+S);
}

template<typename TemplateItem>
const TemplateItem * ScanUntilNot(const TemplateItem Start[], const SIZE_TYPE S, const TemplateItem ItemListStart[], const TemplateItem ItemListEnd[])
{
    return ScanUntilNot(Start,Start+S, ItemListStart, ItemListEnd);
}

template<typename TemplateItem>
const TemplateItem * ScanUntilNot(const TemplateItem Start[], const SIZE_TYPE S1, const TemplateItem ItemListStart[], const SIZE_TYPE S2)
{
    return ScanUntilNot(Start,Start+S1, ItemListStart, ItemListStart+S2);
}

//ScanUntilNot non-const

template<typename TemplateItem>
TemplateItem * ScanUntilNot(TemplateItem Start[], const TemplateItem End[], const TemplateItem &Item)
{
    while(Start != End)
    {
        if(*Start != Item){return Start;}
        Start++;
    }
    return NULL;
}

template<typename TemplateItem>
TemplateItem * ScanUntilNot(TemplateItem Start[], const SIZE_TYPE S, const TemplateItem &Item)
{
    return ScanUntilNot(Start,Start+S,Item);

}

template<typename TemplateItem>
TemplateItem * ScanUntilNot(TemplateItem Start[], const TemplateItem End[], const TemplateItem ItemListStart[], const TemplateItem ItemListEnd[])
{
    while(Start != End)
    {
        if(!CompareList(*Start,ItemListStart,ItemListEnd))
        {
            return Start;
        }
        Start++;
    }
    return NULL;
}

template<typename TemplateItem>
TemplateItem * ScanUntilNot(TemplateItem Start[], const TemplateItem End[], const TemplateItem ItemListStart[], const SIZE_TYPE S)
{
    return ScanUntilNot(Start,End, ItemListStart, ItemListStart+S);
}

template<typename TemplateItem>
TemplateItem * ScanUntilNot(TemplateItem Start[], const SIZE_TYPE S, const TemplateItem ItemListStart[], const TemplateItem ItemListEnd[])
{
    return ScanUntilNot(Start,Start+S, ItemListStart, ItemListEnd);
}

template<typename TemplateItem>
TemplateItem * ScanUntilNot(TemplateItem Start[], const SIZE_TYPE S1, const TemplateItem ItemListStart[], const SIZE_TYPE S2)
{
    return ScanUntilNot(Start,Start+S1, ItemListStart, ItemListStart+S2);
}

template<typename TemplateItem>
const bool IsItemBefore(const TemplateItem &IsThis, const TemplateItem &BeforeThis, const TemplateItem StartItemList[], const TemplateItem EndItemList[])
{
    while(StartItemList != EndItemList)
    {
        if(IsThis == *StartItemList){return true;}
        if(BeforeThis == *StartItemList){return false;}
        ++StartItemList;
    }
    return false;
}

template<typename TemplateItem>
const bool IsItemBefore(const TemplateItem &IsThis, const TemplateItem &BeforeThis, const TemplateItem StartItemList[], const SIZE_TYPE S)
{
    return IsItemBefore(IsThis,BeforeThis,StartItemList,StartItemList+S);
}

template<typename TemplateItem>
const bool IsItemBefore(const TemplateItem &IsThis, const TemplateItem BeforeThisListStart[], const TemplateItem BeforeThisListEnd[], const TemplateItem ItemListStart[], const TemplateItem ItemListEnd[])
{
    while(ItemListStart != ItemListEnd)
    {
        if(IsThis == *ItemListStart){return true;}
        if(CompareList(*ItemListStart,BeforeThisListStart,BeforeThisListEnd)){return false;}
        ++ItemListStart;
    }
    return false;
}

template<typename TemplateItem>
const bool IsItemBefore(const TemplateItem &IsThis, const TemplateItem BeforeThisListStart[], const SIZE_TYPE S, const TemplateItem ItemListStart[], const TemplateItem ItemListEnd[])
{
    return IsItemBefore(IsThis,BeforeThisListStart,BeforeThisListStart+S,ItemListStart,ItemListEnd);
}

template<typename TemplateItem>
const bool IsItemBefore(const TemplateItem &IsThis, const TemplateItem BeforeThisListStart[], const SIZE_TYPE S1, const TemplateItem ItemListStart[], const SIZE_TYPE S2)
{
    return IsItemBefore(IsThis,BeforeThisListStart,BeforeThisListStart+S1,ItemListStart,ItemListStart+S2);
}

template<typename TemplateItem>
const bool ReplaceItem(TemplateItem &Target, const TemplateItem StartItemList[], const TemplateItem EndItemList[], const TemplateItem &ReplaceWith)
{
    if(IsItem(Target,StartItemList,EndItemList))
    {
        Target = ReplaceWith;
        return true;
    }
    return false;
}

template<typename TemplateItem>
const bool ReplaceItem(TemplateItem &Target, const TemplateItem ItemList[], const SIZE_TYPE S, const TemplateItem &ReplaceWith)
{
    return ReplaceItem(Target,ItemList,ItemList+S,ReplaceWith);
}

template<typename TemplateItem>
const bool Replace(TemplateItem Start[], TemplateItem End[], const TemplateItem StartFindItem[], const TemplateItem EndFindItem[], const TemplateItem &ReplaceWith)
{
    bool ReturnType = false;
    while(Start != End)
    {
        if(ReplaceItem(*Start,StartFindItem,EndFindItem,ReplaceWith))
        {
            if(!ReturnType){ReturnType = true;}
        }
        ++Start;
    }
    return ReturnType;
}

template<typename TemplateItem>
const bool Replace(TemplateItem Start[], TemplateItem End[], const TemplateItem FindItem[], const SIZE_TYPE S, const TemplateItem &ReplaceWith)
{
    return Replace(Start,End,FindItem,FindItem+S,ReplaceWith);
}

template<typename TemplateItem>
const bool Replace(TemplateItem Start[], const SIZE_TYPE S, const TemplateItem FindItem[], const SIZE_TYPE S2, const TemplateItem &ReplaceWith)
{
    return Replace(Start,Start+S,FindItem,FindItem+S2,ReplaceWith);
}

template<typename TemplateItem>
const std::vector<TemplateItem> ToVector(const TemplateItem Data[], const SIZE_TYPE S)
{
    std::vector<TemplateItem> Temp;
    if( (Data == NULL) || (S < 1) )
    {
        return Temp;
    }

    try
    {
        Temp.resize(S);
    }
    catch(...)
    {
        std::vector<TemplateItem> Temp2;
        return Temp2;
    }

    Copy(&Temp[0],Data,S);
    return Temp;
}

const std::string ToString(const char Data[], const SIZE_TYPE S)
{
    std::string Temp;
    if( (Data == NULL) || (S < 1) )
    {
        return Temp;
    }

    try
    {
        Temp.resize(S);
    }
    catch(...)
    {
        //std::string Temp2;
        return std::string();
    }

    Copy(&Temp[0],Data,S);
    return Temp;
}

template<typename TemplateItem>
inline const bool IsValidArray(const TemplateItem Data[], const SIZE_TYPE S){ return ((Data != NULL)&&(S > 0)); }

template<typename TemplateItem>
inline void Swap(TemplateItem *&ItemCopy, TemplateItem *&ItemCopy2)
{
    TemplateItem *Temp = ItemCopy2;
    ItemCopy2 = ItemCopy;
    ItemCopy = Temp;
    return;
}

template<typename TemplateItem>
inline void Swap(TemplateItem *&ItemCopy, SIZE_TYPE &S1, TemplateItem *&ItemCopy2, SIZE_TYPE &S2)
{
    Swap(ItemCopy,ItemCopy2);
    SIZE_TYPE Temp = S2;
    S2 = S1;
    S1 = Temp;
    return;
}

template<typename TemplateItem>
inline const SIZE_TYPE Length(const TemplateItem Start[], const TemplateItem &EndValue)
{
    if(Start == NULL){return 0;}
    const TemplateItem *Temp = Start;
    while(*Temp != EndValue){++Temp;}
    return (Temp-Start)+1;
}
template<typename TemplateItem>
const bool Compare(const TemplateItem CompareStart[], const TemplateItem CompareEnd[], const TemplateItem Start[], const TemplateItem End[])
{
    if((CompareStart == NULL) || (CompareEnd == NULL) || (Start == NULL) || (End == NULL)){return false;}
    if( (CompareEnd-CompareStart) != (End-Start) ){return false;}
    while( (CompareStart != CompareEnd) && (Start != End) )
    {
        if(*CompareStart != *Start){return false;}
        ++CompareStart;
        ++Start;
    }
    return true;
}

template<typename TemplateItem>
inline const bool Compare(const TemplateItem CompareStart[], const SIZE_TYPE S1, const TemplateItem Start[], const SIZE_TYPE S2)
{
    return Compare(CompareStart,CompareStart+S1,Start,Start+S2);
}

const bool Compare(const char CompareStart[], const char CompareEnd[], const char Start[], const char End[], const bool CaseInsensitive)
{
    if((CompareStart == NULL) || (CompareEnd == NULL) || (Start == NULL) || (End == NULL)){return false;}
    if( (CompareEnd-CompareStart) != (End-Start) ){return false;}
    while( (CompareStart != CompareEnd) && (Start != End) )
    {
        if(*CompareStart != *Start)
        {
            if(!CaseInsensitive){return false;}
            else
            {
                if(*CompareStart >= 'A' && *CompareStart <= 'Z')
                {
                    if( (*CompareStart+32) != *Start)
                    {
                        return false;
                    }
                }

                if(*CompareStart >= 'a' && *CompareStart <= 'z')
                {
                    if( (*CompareStart-32) != *Start)
                    {
                        return false;
                    }
                }
            }
        }
        ++CompareStart;
        ++Start;
    }
    return true;
}

inline const bool Compare(const char CompareStart[], const SIZE_TYPE S1, const char Start[], const SIZE_TYPE S2, const bool CaseInsensitive)
{
    return Compare(CompareStart,CompareStart+S1,Start,Start+S2,CaseInsensitive);
}

template<typename TemplateItem>
const bool Compare(const TemplateItem CompareStart[], const TemplateItem To[], const bool CaseInsensitive)
{
    return Compare(CompareStart,strlen(CompareStart),To,strlen(To),CaseInsensitive);
}


//Is 'IsCompareArray' a version of 'FindItem'?
template<typename TemplateItem>
inline const bool IsCompareArray(const TemplateItem &Item, const TemplateItem CompareTo[], const TemplateItem &EndValue)
{
    if(!CompareTo){return false;}
    while(*CompareTo != EndValue)
    {
        if(Item == *CompareTo){return true;}
        ++CompareTo;
    }
    return false;
}

template<typename TemplateItem>
inline const bool IsCompareArray(const TemplateItem &Item, const TemplateItem CompareTo[], const SIZE_TYPE S)
{
    if(!CompareTo){return false;}
    const TemplateItem *TempEnd = (CompareTo+S);
    while(CompareTo != TempEnd)
    {
        if(Item == *CompareTo){return true;}
        ++CompareTo;
    }
    return false;
}

inline const bool IsChar(const char &Target, const char &Source, const bool CaseInsensitive = false)
{
    if(Target == Source){return true;}
    if(CaseInsensitive)
    {
        if( (Source >= 'A') && (Source <= 'Z') )
        {
            return (Target == (Source+32));
        }

        if( (Source >= 'a') && (Source <= 'z') )
        {
            return (Target == (Source-32));
        }
    }
    return false;
}

inline const bool IsCompareChar(const char &Target, const char CompareTo[], const bool CaseInsensitive = false, const bool AndNull = false)
{
    while(*CompareTo != '\0')
    {
        if(IsChar(Target,*CompareTo,CaseInsensitive)){return true;}
        ++CompareTo;
    }
    if(AndNull){ if(IsChar(Target,*CompareTo,CaseInsensitive)){return true;} }
    return false;
}

inline const char * IsCompareArrayChar(const char Target[], const char CompareTo[], const bool CaseInsensitive = false, const bool AndNull = false)
{
    while(Target != '\0')
    {
        if(IsCompareChar(*Target,CompareTo,CaseInsensitive,AndNull))
        {
            return Target;
        }
        ++Target;
    }
    return NULL;
}

inline char * IsCompareArrayChar(char Target[], const char CompareTo[], const bool CaseInsensitive = false, const bool AndNull = false)
{
    while(Target != '\0')
    {
        if(IsCompareChar(*Target,CompareTo,CaseInsensitive,AndNull))
        {
            return Target;
        }
        ++Target;
    }
    return NULL;
}

inline char * IsCompareArrayChar(char TargetStart[], const char TargetEnd[], const char CompareTo[], const bool CaseInsensitive = false, const bool AndNull = false)
{
    while(TargetStart != TargetEnd)
    {
        if(IsCompareChar(*TargetStart,CompareTo,CaseInsensitive,AndNull))
        {
            return TargetStart;
        }
        ++TargetStart;
    }
    return NULL;
}

inline const char * IsCompareArrayChar(const char TargetStart[], const char TargetEnd[], const char CompareTo[], const bool CaseInsensitive = false, const bool AndNull = false)
{
    while(TargetStart != TargetEnd)
    {
        if(IsCompareChar(*TargetStart,CompareTo,CaseInsensitive,AndNull))
        {
            return TargetStart;
        }
        ++TargetStart;
    }
    return NULL;
}

inline char * IsCompareArrayChar(char Target[], const SIZE_TYPE S, const char CompareTo[], const bool CaseInsensitive = false, const bool AndNull = false)
{
    return IsCompareArrayChar(Target,Target+S,CompareTo,CaseInsensitive,AndNull);
}


inline const char * IsCompareArrayChar(const char Target[], const SIZE_TYPE S, const char CompareTo[], const bool CaseInsensitive = false, const bool AndNull = false)
{
    return IsCompareArrayChar(Target,Target+S,CompareTo,CaseInsensitive,AndNull);
}

//

inline char * IsCompareArrayCharBackwards(const char TargetStart[], char TargetEnd[], const char CompareTo[], const bool CaseInsensitive = false, const bool AndNull = false)
{
    while(TargetStart != TargetEnd)
    {
        if(IsCompareChar(*TargetEnd,CompareTo,CaseInsensitive,AndNull))
        {
            return TargetEnd;
        }
        TargetEnd--;
    }
    return NULL;
}

inline const char * IsCompareArrayCharBackwards(const char TargetStart[], const char TargetEnd[], const char CompareTo[], const bool CaseInsensitive = false, const bool AndNull = false)
{
    while(TargetStart != TargetEnd)
    {
        if(IsCompareChar(*TargetEnd,CompareTo,CaseInsensitive,AndNull))
        {
            return TargetEnd;
        }
        TargetEnd--;
    }
    return NULL;
}

inline const char * IsCompareArrayCharBackwards(const char Target[], const char CompareTo[], const bool CaseInsensitive = false, const bool AndNull = false)
{
    return IsCompareArrayCharBackwards(Target,(Target+strlen(Target)),CompareTo,CaseInsensitive,AndNull);
}

inline char * IsCompareArrayCharBackwards(char Target[], const SIZE_TYPE S, const char CompareTo[], const bool CaseInsensitive = false, const bool AndNull = false)
{
    return IsCompareArrayCharBackwards(Target,Target+S,CompareTo,CaseInsensitive,AndNull);
}

inline const char * IsCompareArrayCharBackwards(const char Target[], const SIZE_TYPE S, const char CompareTo[], const bool CaseInsensitive = false, const bool AndNull = false)
{
    return IsCompareArrayCharBackwards(Target,Target+S,CompareTo,CaseInsensitive,AndNull);
}

inline char * IsCompareArrayCharBackwards(char Target[], const char CompareTo[], const bool CaseInsensitive = false, const bool AndNull = false)
{
    return IsCompareArrayCharBackwards(Target,strlen(Target),CompareTo,CaseInsensitive,AndNull);
}

const std::string ExtractSubString(const char Search[], const SIZE_TYPE S, const char Start[], const SIZE_TYPE S2, const char End[], const SIZE_TYPE S3, const bool RemoveSearch = false)
{
    ERRORFUNCTION(ExtractSubString,!IsValidArray(Search,S),return std::string();)
    ERRORFUNCTION(ExtractSubString,!IsValidArray(Start,S2),return std::string();)
    ERRORFUNCTION(ExtractSubString,!IsValidArray(End,S3),return std::string();)

    const char * C_Ptr = Find(Search,S,Start,S2,RemoveSearch), * C_Ptr_2 = NULL;
    if(C_Ptr == NULL){return std::string();}
    if(RemoveSearch){C_Ptr++;}
    C_Ptr_2 = Find(C_Ptr+((S2-1) * !RemoveSearch),Search+S,End,S3,!RemoveSearch);
    if(C_Ptr_2 == NULL){return std::string();}
    if(RemoveSearch){C_Ptr_2--;}

    return ToString(C_Ptr,(C_Ptr_2-C_Ptr)+1);
}

const std::string ExtractSubString(const char Search[], const char Start[], const char End[], const bool RemoveSearch = false)
{
    ERRORFUNCTION(ExtractSubString,!Search,return std::string();)
    ERRORFUNCTION(ExtractSubString,!Start,return std::string();)
    ERRORFUNCTION(ExtractSubString,!End,return std::string();)
    return ExtractSubString(Search,strlen(Search),Start,strlen(Start),End,strlen(End),RemoveSearch);
}

const bool InsertStringRepeat(char Target[], const char Array[], const char EndArray[], const char Insert[], const char EndInsert[], const unsigned int &Spacing)
{
    ERRORFUNCTION(InsertStringRepeat,!Target,RETURN_BOOL)
    ERRORFUNCTION(InsertStringRepeat,!Array,RETURN_BOOL)
    ERRORFUNCTION(InsertStringRepeat,!EndArray,RETURN_BOOL)
    ERRORFUNCTION(InsertStringRepeat,!Insert,RETURN_BOOL)
    ERRORFUNCTION(InsertStringRepeat,!EndInsert,RETURN_BOOL)

    if(Spacing > 0)
    {
        const char * C_Ptr = Array;
        char * C_Ptr_T2 = Target;
        while(C_Ptr < EndArray)
        {
            if(Spacing > (unsigned)(EndArray-C_Ptr) )
            {
                C_Ptr_T2 = Copy(C_Ptr_T2,C_Ptr,EndArray-C_Ptr);
                C_Ptr = C_Ptr+(EndArray-C_Ptr);
            }
            else
            {
                C_Ptr_T2 = Copy(C_Ptr_T2,C_Ptr,Spacing);
                C_Ptr = C_Ptr+Spacing;
                C_Ptr_T2 = Copy(C_Ptr_T2,Insert,EndInsert);
            }
        }
    }
    else
    {
        Copy(Target,Array,EndArray);
    }

    return true;
}

const bool InsertStringRepeat(char Target[], const char Array[], const SIZE_TYPE &S, const char Insert[], const SIZE_TYPE &S2, const unsigned int &Spacing)
{
    ERRORFUNCTION(InsertStringRepeat,!Target,RETURN_BOOL)
    ERRORFUNCTION(InsertStringRepeat,!Array,RETURN_BOOL)
    ERRORFUNCTION(InsertStringRepeat,S < 1,RETURN_BOOL)
    ERRORFUNCTION(InsertStringRepeat,!Insert,RETURN_BOOL)
    ERRORFUNCTION(InsertStringRepeat,S2 < 1,RETURN_BOOL)
    return InsertStringRepeat(Target,Array,Array+S,Insert,Insert+S2,Spacing);
}

const std::string InsertStringRepeat(const char Array[], const SIZE_TYPE S, const char Insert[], const SIZE_TYPE S2, const unsigned int Spacing)
{

    ERRORFUNCTION(InsertStringRepeat,!Array,return std::string();)
    ERRORFUNCTION(InsertStringRepeat,S < 1,return std::string();)
    ERRORFUNCTION(InsertStringRepeat,!Insert,return std::string();)
    ERRORFUNCTION(InsertStringRepeat,S2 < 1, return std::string();)

    std::string Data;

    try
    {
        if(Spacing < 1)
        {
            Data.resize(S);
        }
        else
        {
            Data.resize(S+(S2*(S/Spacing)));
        }
    }
    catch(...)
    {
        ERRORFUNCTION(InsertStringRepeat,1,return std::string();)
    }

    InsertStringRepeat(&Data[0],Array,Array+S,Insert,Insert+S2,Spacing);
    return Data;
}

const bool RemoveStringRepeat(char Target[], const char Array[], const SIZE_TYPE S, const char Insert[], const SIZE_TYPE S2)
{
    ERRORFUNCTION(RemoveStringRepeat,!Target,RETURN_BOOL)
    ERRORFUNCTION(RemoveStringRepeat,!Array,RETURN_BOOL)
    ERRORFUNCTION(RemoveStringRepeat,S < 1,RETURN_BOOL)
    ERRORFUNCTION(RemoveStringRepeat,!Insert,RETURN_BOOL)
    ERRORFUNCTION(RemoveStringRepeat,S2 < 1,RETURN_BOOL)

    const char *C_Ptr = Array;
    const char * TargetIter = Array;
    const char * TargetEnd = Array+S;
    char * DataIter = Target;

    while(TargetIter < TargetEnd)
    {
        if(!(TargetIter = Find(TargetIter,TargetEnd,Insert,S2,false))){ break; }
        DataIter = Copy(DataIter,C_Ptr,TargetIter-C_Ptr);
        C_Ptr = TargetIter + S2;
        TargetIter = TargetIter + S2 + 1;
    }

    return true;
}

const bool RemoveStringRepeat(char Target[], const char Array[], const char EndArray[], const char Insert[], const char EndInsert[])
{
    ERRORFUNCTION(RemoveStringRepeat,!Target,RETURN_BOOL)
    ERRORFUNCTION(RemoveStringRepeat,!Array,RETURN_BOOL)
    ERRORFUNCTION(RemoveStringRepeat,!EndArray,RETURN_BOOL)
    ERRORFUNCTION(RemoveStringRepeat,!Insert,RETURN_BOOL)
    ERRORFUNCTION(RemoveStringRepeat,!EndInsert,RETURN_BOOL)

    return RemoveStringRepeat(Target,Array,EndArray-Array,Insert,EndInsert-Insert);
}

const std::string RemoveStringRepeat(const char Array[], const SIZE_TYPE S, const char Insert[], const SIZE_TYPE S2)
{
    SIZE_TYPE N = 0;
    const char * C_Ptr = Array;
    while( (C_Ptr = Find(C_Ptr,S,Insert,S2,true)) ){ N++; }
    std::string Data;

    try
    {
        Data.resize(S-(S2*N));
    }
    catch(...)
    {
        ERRORFUNCTION(RemoveStringRepeat,1,return std::string();)
    }

    RemoveStringRepeat(&Data[0],Array,S,Insert,S2);
    return Data;
}

const SIZE_TYPE MeasureBetweenPoints(const char SearchIn[], const SIZE_TYPE Length1, const char StartTerm[], const SIZE_TYPE Length2, const char EndTerm[], const SIZE_TYPE Length3, const bool IncludeSearch)
{
    ERRORFUNCTION(MeasureBetweenPoints,!IsValidArray(SearchIn,Length1),return 0;)
    ERRORFUNCTION(MeasureBetweenPoints,!IsValidArray(StartTerm,Length2),return 0;)
    ERRORFUNCTION(MeasureBetweenPoints,!IsValidArray(EndTerm,Length3),return 0;)

    const char * ItemCopy1 = Find(SearchIn,Length1,StartTerm,Length2,false);

    ERRORFUNCTION(MeasureBetweenPoints,!ItemCopy1,return 0;)

    const char * ItemCopy2 = Find(ItemCopy1+Length2,SearchIn+Length1,EndTerm,Length3,true);
    ERRORFUNCTION(MeasureBetweenPoints,!ItemCopy2,return 0;)
    ItemCopy2++;

    if(IncludeSearch)
    {
        return ItemCopy2-ItemCopy1;
    }
    return (ItemCopy2-ItemCopy1)-(Length2+Length3);
}

const SIZE_TYPE MeasureBetweenPoints(const char SearchIn[], const char StartTerm[], const char EndTerm[], const bool IncludeSearchTerms)
{
    ERRORFUNCTION(MeasureBetweenPoints,!SearchIn,return 0;)
    ERRORFUNCTION(MeasureBetweenPoints,!StartTerm,return 0;)
    ERRORFUNCTION(MeasureBetweenPoints,!EndTerm,return 0;)
    return MeasureBetweenPoints(SearchIn,strlen(SearchIn),StartTerm,strlen(StartTerm),EndTerm,strlen(EndTerm),IncludeSearchTerms);
}

const bool IsNumericString(const char Array[])
{
    while(*Array != '\0')
    {
        if(!IsCompareChar(*Array,"0123456789"))
        {
            return false;
        }
        Array++;
    }
    return true;
}

const bool IsNumericSymbols(const char Array[])
{
    if(*Array == '-')
    {
        Array++;
    }

    bool Temp = false;
    while(*Array != '\0')
    {
        if(!IsCompareChar(*Array,"0123456789"))
        {
            if(*Array == '.' && !Temp)
            {
                Temp = true;
            }
            else
            {
                return false;
            }
        }
        Array++;
    }
    return true;
}

const bool IsAlphaNumericString(const char Array[])
{
    while(*Array != '\0')
    {
        if(!IsCompareChar(*Array,"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789"))
        {
            return false;
        }
        Array++;
    }
    return true;
}

const bool IsAlpha(const char C)
{
    return IsCompareChar(C,"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM");
}

const bool IsAlphaNumeric(const char C)
{
    return IsCompareChar(C,"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789");
}

const bool IsNumeric(const char C)
{
    return IsCompareChar(C,"0123456789");
}

const bool IsIPv4(const char C)
{
    return IsCompareChar(C,"0123456789.:");
}

const bool IsIPv6(const char C)
{
    return IsCompareChar(C,"0123456789.:ABCDEF");
}

const bool ArrayToUpper(char Array[], const SIZE_TYPE IC1)
{
    if(!IsValidArray(Array,IC1)){return false;}
    SIZE_TYPE Iter = 0;

    while(Iter < IC1)
    {
        if(islower(Array[Iter]))
        {
            Array[Iter] = toupper(Array[Iter]);
        }
        Iter++;
    }
    return true;
}

const bool ArrayToLower(char Array[], const SIZE_TYPE IC1)
{
    if(!IsValidArray(Array,IC1)){return false;}
    SIZE_TYPE Iter = 0;

    while(Iter < IC1)
    {
        if(isupper(Array[Iter]))
        {
            Array[Iter] = tolower(Array[Iter]);
        }
        Iter++;
    }
    return true;
}

const bool IsArray(const char Array[], const SIZE_TYPE S, const bool (*CharFunction)(const char))
{
    if(!Array){return false;}
    if(S < 1){return false;}

    const char * C_Ptr = Array;
    while(C_Ptr != (Array+S))
    {
        if(!CharFunction(*C_Ptr))
        {
            return false;
        }
        C_Ptr++;
    }
    return true;
}

const bool IsArray(const char Array[], const bool (*CharFunction)(const char))
{
    if(!Array){return false;}
    return IsArray(Array,strlen(Array),CharFunction);
}

const bool IsArray(const char Array[], const SIZE_TYPE S, int (*CharFunction)(int), const bool NotChar = false)
{
    if(!Array){return false;}
    if(S < 1){return false;}

    const char * C_Ptr = Array;
    while(C_Ptr != (Array+S))
    {
        if(!CharFunction(*C_Ptr))
        {
            return false;
        }
        C_Ptr++;
    }
    return true;
}

const bool IsArray(const char Array[], int (*CharFunction)(int), const bool NotChar = false)
{
    if(!Array){return false;}
    return IsArray(Array,strlen(Array),CharFunction,NotChar);
}

const std::string IntToString(const int Int)
{
    char Array[30];
    sprintf(Array,"%d",Int);
    return ToString(Array,strlen(Array));
}

const std::string FloatToString(const float Float)
{
    std::string String;
    #ifdef IS_WINDOWS
    String.resize(_snprintf(NULL,0,"%f",Float)+1);
    #else
    String.resize(snprintf(NULL,0,"%f",Float)+1);
    #endif
    sprintf(&String[0],"%f",Float);
    return String;
}

const long StringToLong(const char String[])
{
    if(!String){return 0;}
    return strtol(String,NULL,10);
}

const float StringToFloat(const char String[])
{
    if(!String){return 0;}
    return strtof(String,NULL);
}

const double StringToDouble(const char String[])
{
    if(!String){return 0;}
    return strtod(String,NULL);
}

const SIZE_TYPE CountNumberOfItems(const char ItemCopy[], const char ItemCopyEnd[], const char FindItem[], const char FindEnd[])
{
    ERRORFUNCTION(CountNumberOfItems,!ItemCopy,return 0;)
    ERRORFUNCTION(CountNumberOfItems,!ItemCopyEnd,return 0;)
    ERRORFUNCTION(CountNumberOfItems,!FindItem,return 0;)
    ERRORFUNCTION(CountNumberOfItems,!FindEnd,return 0;)
    ERRORFUNCTION(CountNumberOfItems,ItemCopy > ItemCopyEnd,return 0;)
    ERRORFUNCTION(CountNumberOfItems,FindItem > FindEnd,return 0;)
    ERRORFUNCTION(CountNumberOfItems,ItemCopy == ItemCopyEnd,return 0;)
    ERRORFUNCTION(CountNumberOfItems,FindItem == FindEnd,return 0;)

    const char * Iterator = ItemCopy;
    SIZE_TYPE Iter = 0;

    while((Iterator = Find(Iterator,ItemCopyEnd,FindItem,FindEnd,true)) )
    {
        Iterator++;
        Iter++;
    }
    return Iter;
}

const SIZE_TYPE CountNumberOfItems(const char ItemCopy[], const SIZE_TYPE Itemcopy, const char FindItem[], const SIZE_TYPE FindEnd)
{
    ERRORFUNCTION(CountNumberOfItems,Itemcopy < 1,return 0;)
    ERRORFUNCTION(CountNumberOfItems,FindEnd < 1,return 0;)
    return CountNumberOfItems(ItemCopy,ItemCopy+Itemcopy,FindItem,FindItem+FindEnd);
}

const SIZE_TYPE CountNumberOfItems(const char ItemCopy[], const char FindItem[])
{
    ERRORFUNCTION(CountNumberOfItems,!ItemCopy,return 0;)
    ERRORFUNCTION(CountNumberOfItems,!FindItem,return 0;)

    return CountNumberOfItems(ItemCopy,strlen(ItemCopy),FindItem,strlen(FindItem));
}

//0.0.0.0:0
//255.255.255.255:65535
//Max length wih port is 21
//Without is 15
const uint8_t IsArrayIPv4(const char Array[], const bool WithPort = false)
{
    if(!Array){return 2;}
    
    if(!IsNumeric(Array[0]))
    {
        return 3;
    }
    
    bool PortMode = false;
    int Iter = 0;
    uint8_t NumberCount = 0, DotCount = 0;
    char Numeric[7];
    
    //While not parsing
    while(Array[Iter] != '\0')
    {
        //Is it a valid IPv4 symbol?
        if(!IsIPv4(Array[Iter]))
        {
            //It's not, return error code
            return 4;
        }
        
        //Is it numeric?
        if(IsNumeric(Array[Iter]))
        {
            //Are we counting a port?
            if(PortMode)
            {
                Numeric[NumberCount] = Array[Iter];
                NumberCount++;
                if(NumberCount > 5)
                {
                    return 10;
                }
            }
            else
            {
                //We tally the numbers to check it's 0-255 range
                Numeric[NumberCount] = Array[Iter];
                NumberCount++;
                //Too many numbers?
                if(NumberCount > 3)
                {
                    return 6;
                }
            }
        }
        else
        {
            if(Array[Iter] == '.')
            {
                if(NumberCount == 3)
                {
                    //If three numbers, must range from 100-255, 200-255 specifically
                    if(Array[Iter-3] != '2' && Array[Iter-3] != '1')
                    {
                        return 11;
                    }
                    
                    if(Array[Iter-3] == 2)
                    {
                    
                        if(!IsCompareChar(Array[Iter-2],"012345"))
                        {
                            return 11;
                        }
                        
                        if(!IsCompareChar(Array[Iter-1],"012345"))
                        {
                            return 11;
                        }
                    }
                }
                else if(NumberCount == 2)
                {
                    //The number before the other one cannot be a zero
                    if(Array[Iter-2] == '0')
                    {
                        return 11;
                    }
                }
                NumberCount = 0;
                
                DotCount++;
                if(DotCount > 3)
                {
                    return 7;
                }
            }
            else
            {
                if(Array[Iter] == ':')
                {
                    //Are ports allowed?
                    if(WithPort)
                    {
                        if(DotCount < 3)
                        {
                            return 9;
                        }
                        PortMode = true;
                    }
                    else
                    {
                        return 8;
                    }
                }
                NumberCount = 0;
            }
        }
        
        if(WithPort)
        {
            if(Iter > 21)
            {
                return 5;
            }
        }
        else
        {
            if(Iter > 15)
            {
                return 5;
            }
        }
        
        Iter++;
    }
    
    //Smallest any IP can be
    if(Iter < 7)
    {
        return 13;
    }
    
    //Final check
    if(PortMode)
    {
        Numeric[NumberCount] = '\0';
        //Much easier than a complicated if/else set-up
        if(StringToLong(Numeric) > 65535)
        {
            return 12;
        }
    }
    //This is very likely an actual IP!
    return 0;
}

template<typename TemplateItem>
const SIZE_TYPE ScanUntilNumeric(const TemplateItem Start[], const TemplateItem End[], const TemplateItem &Item)
{
    if(!Start)
    {
        return 0;
    }
    SIZE_TYPE Iter = 1;
    while( &(Start[Iter]) != End)
    {
        if(Start[Iter] == Item){return Iter;}
        Iter++;
    }
    return 0;
}

template<typename TemplateItem>
const SIZE_TYPE ScanUntilNumeric(const TemplateItem Start[], const SIZE_TYPE L1, const TemplateItem &Item)
{
    return ScanUntilNumeric(Start,Start+L1,Item);
}

const SIZE_TYPE ScanUntilNumeric(const char Start[], const char &Item)
{
    if(!Start)
    {
        return 0;
    }
    return ScanUntilNumeric(Start,Start+strlen(Start),Item);
}



}

#endif
