#ifndef G_RANDOM_H
#define G_RANDOM_H

/*
Generated on: 2012-08-08-15.47.56

Version: 4.2
*/

/*
Notes:
GetRandomUnsignedInt with ability to take range arguments added

Improved boolean randomisation and made more efficient

Modified GetRandomString, which now includes extra overhead in memory but ultimately saves
Many, many CPU passes of 'guessing' the correct char variable (giving a huge CPU efficiency saving on bulk tasks)

Added void versions of int randomisation functions and removed optional arguments
This offers greater function diversity for RandomImage's capabilities

GetRandom changed to GetRandomInt and GetRandomFloat
Added GetRandomInt functions for various computations

SFML discontinued support for srand etc, which have now been replaced by C++ calls
*/

/*
Suggestions for improvements:


*/

#include <climits>
#include <cctype>
#include <stdlib.h> //srand, rand
#include <ctime>
#include "ErrorMessage.h"
#include "CharArray.h"
#include "Maths.h"
#include "TemplateList.h"


namespace TL
{

const bool GetRandomBool()
{
    return (clock()&7&1&5);
}

const int GetRandomInt()
{
    int Temp = 0;

    for(unsigned int I = 0;I < (sizeof(int)*8);I++)
    {
        Temp = Temp + (GetRandomBool() << I);
    }
    return Temp;
}

const unsigned int GetRandomUnsignedInt()
{
    unsigned int Temp = 0;

    for(unsigned int I = 0;I < (sizeof(unsigned int)*8);I++)
    {
        Temp = Temp + (GetRandomBool() << I);
    }
    return Temp;
}

const unsigned int GetRandomUnsignedInt(const unsigned int Start, const unsigned int End)
{
    srand(GetRandomUnsignedInt());

    return BetweenRangeUnsignedInt(rand(),Start,End);
}

const short GetRandomShort()
{
    short Temp = 0;

    for(unsigned int I = 0;I < (sizeof(short)*8);I++)
    {
        Temp = Temp + (GetRandomBool() << I);
    }

    return Temp;
}

const unsigned short GetRandomUnsignedShort()
{
    unsigned short Temp = 0;

    for(unsigned int I = 0;I < (sizeof(unsigned short)*8);I++)
    {
        Temp = Temp + (GetRandomBool() << I);
    }

    return Temp;
}

const int GetRandomXORInt(const int X)
{
    return GetRandomInt() ^ X;
}

const int GetRandomAddedInt(const int X)
{
    return GetRandomInt() + X;
}

const int GetRandomSubtractedInt(const int X)
{
    return GetRandomInt() - X;
}

const int GetRandomMultipliedInt(const int X)
{
    int Y = GetRandomInt();
    if(X == 0){return 0;}
    if(Y == 0){return 0;}
    return X*Y;
}

const int GetRandomDividedInt(const int X)
{
    int Y = GetRandomInt();
    if(X == 0){return 0;}
    if(Y == 0){return 0;}
    return X/Y;
}

const int GetRandomANDInt(const int X)
{
    return (GetRandomInt())&X;
}

const int GetRandomORInt(const int X)
{
    return (GetRandomInt())|X;
}

/*
*/

const int GetRandomInvertedInt()
{
    return ~(GetRandomInt());
}

const int GetRandomXORInt()
{
    return GetRandomInt() ^ GetRandomInt();
}

const int GetRandomAddedInt()
{
    return GetRandomInt() + GetRandomInt();
}

const int GetRandomSubtractedInt()
{
    return GetRandomInt() - GetRandomInt();
}

const int GetRandomMultipliedInt()
{
    int Y = GetRandomInt();
    int X = GetRandomInt();
    if(X == 0){return 0;}
    if(Y == 0){return 0;}
    return X*Y;
}

const int GetRandomDividedInt()
{
    int Y = GetRandomInt();
    int X = GetRandomInt();
    if(X == 0){return 0;}
    if(Y == 0){return 0;}
    return X/Y;
}

const int GetRandomANDInt()
{
    return (GetRandomInt())&GetRandomInt();
}

const int GetRandomORInt()
{
    return (GetRandomInt())|(GetRandomInt());
}

const char GetRandomChar()
{
    char Temp = 0;

    for(unsigned int I = 0;I < (sizeof(char)*8);I++)
    {
        Temp = Temp + (GetRandomBool() << I);
    }

    return Temp;
}

const int GetRandomInt(const int Start, const int End)
{
    srand(GetRandomInt());

    return BetweenRange(rand(),Start,End);
}

const float GetRandomFloat()
{
    srand(GetRandomInt());
    int Temp = rand();
    srand(GetRandomInt());
    return ((rand()/(Temp+3.141))*rand())/10000;
}

const float GetRandomFloat(const float Start, const float End)
{
    return BetweenRangeFloat(GetRandomFloat(),Start,End);
}

const int GetRandomAverage(const int Start, const int End, const unsigned int Averages)
{
    if(Averages == 0){return 0;}
    int Temp = 0;
    for(unsigned int I = 0; I < Averages;I++)
    {
        Temp = Temp + GetRandomInt(Start,End);
    }
    if(Temp == 0){return 0;}
    if(Temp < 0)
    {
        Temp = abs(Temp);
        Temp = Temp/Averages;

        return Temp - (Temp*2);
    }
    return (Temp/Averages);
}

const bool GetRandomString(char Array[], const SIZE_TYPE S, const char CharStart, const char CharEnd, bool (*CharFunction)(const char C))
{
    ERRORFUNCTION(GetRandomString,!Array,RETURN_BOOL)
    ERRORFUNCTION(GetRandomString,S < 1,RETURN_BOOL)

    TemplateArray<char> Acceptable;

    //Clears from memory scope once done
    {
        TemplateList<char> AcceptableChars;
        char Char = CHAR_MIN;

        while(1)
        {
            if(CharFunction(Char))
            {
                ERRORFUNCTION(GetRandomString,!AcceptableChars.Append(Char),RETURN_BOOL)
            }
            if(Char == CHAR_MAX){break;}
            Char++;
        }

        ERRORFUNCTION(GetRandomString,!AcceptableChars.IsValid(),RETURN_BOOL)
        ERRORFUNCTION(GetRandomString,!Acceptable.SetSize(AcceptableChars.GetSize()),RETURN_BOOL)
        AcceptableChars.CurrToStart();

        do{
            Acceptable[AcceptableChars.GetIter()-1] = AcceptableChars.GetCurrItem();
        }while(AcceptableChars.CurrNext());
    }


    SIZE_TYPE Temp = 0;

    while(Temp < S)
    {
        Array[Temp] = Acceptable[GetRandomInt(0,Acceptable.GetSize())];
        Temp++;
    }

    return true;
}

const bool GetRandomString(char Array[], const SIZE_TYPE S, const int CharStart, const int CharEnd, int (*CharFunction)(const int C))
{
    ERRORFUNCTION(GetRandomString,!Array,RETURN_BOOL)
    ERRORFUNCTION(GetRandomString,S < 1,RETURN_BOOL)

    TemplateArray<char> Acceptable;

    //Clears from memory scope once done
    {
        TemplateList<char> AcceptableChars;
        char Char = CHAR_MIN;

        while(1)
        {
            if(CharFunction(Char))
            {
                ERRORFUNCTION(GetRandomString,!AcceptableChars.Append(Char),RETURN_BOOL)
            }
            if(Char == CHAR_MAX){break;}
            Char++;
        }

        ERRORFUNCTION(GetRandomString,!AcceptableChars.IsValid(),RETURN_BOOL)
        ERRORFUNCTION(GetRandomString,!Acceptable.SetSize(AcceptableChars.GetSize()),RETURN_BOOL)
        AcceptableChars.CurrToStart();

        do{
            Acceptable[AcceptableChars.GetIter()-1] = AcceptableChars.GetCurrItem();
        }while(AcceptableChars.CurrNext());
    }


    SIZE_TYPE Temp = 0;

    while(Temp < S)
    {
        Array[Temp] = Acceptable[GetRandomInt(0,Acceptable.GetSize())];
        Temp++;
    }

    return true;
}

template<typename TemplateItem>
const TemplateItem GetItemFromListRandom(const int RandomNumber, const int NumberOfItems, const TemplateItem ItemCopy2, ...)
{
    ERRORFUNCTION(GetItemFromListRandom,RandomNumber >= NumberOfItems,return TemplateItem();)
    return (&ItemCopy2)[RandomNumber];
}

const bool GetRandomString(char Array[], const SIZE_TYPE S, const bool (*CharFunction)(const char C))
{
    ERRORFUNCTION(GetRandomString,!Array,RETURN_BOOL)
    ERRORFUNCTION(GetRandomString,S < 1,RETURN_BOOL)

    TemplateArray<char> Acceptable;

    //Clears from memory scope once done
    {
        TemplateList<char> AcceptableChars;
        char Char = CHAR_MIN;

        while(1)
        {
            if(CharFunction(Char))
            {
                ERRORFUNCTION(GetRandomString,!AcceptableChars.Append(Char),RETURN_BOOL)
            }
            if(Char == CHAR_MAX){break;}
            Char++;
        }

        ERRORFUNCTION(GetRandomString,!AcceptableChars.IsValid(),RETURN_BOOL)
        ERRORFUNCTION(GetRandomString,!Acceptable.SetSize(AcceptableChars.GetSize()),RETURN_BOOL)
        AcceptableChars.CurrToStart();

        do{
            Acceptable[AcceptableChars.GetIter()-1] = AcceptableChars.GetCurrItem();
        }while(AcceptableChars.CurrNext());
    }


    SIZE_TYPE Temp = 0;

    while(Temp < S)
    {
        Array[Temp] = Acceptable[GetRandomInt(0,Acceptable.GetSize())];
        Temp++;
    }

    return true;
}

//This accepts a point to a char function to define what type of characters you want in the random string
//This is the int version, designed for compatibility with C type functions
const bool GetRandomString(char Array[], const SIZE_TYPE S, int (*CharFunction)(const int C))
{
    ERRORFUNCTION(GetRandomString,!Array,RETURN_BOOL)
    ERRORFUNCTION(GetRandomString,S < 1,RETURN_BOOL)

    TemplateArray<char> Acceptable;

    //Clears from memory scope once done
    {
        TemplateList<char> AcceptableChars;
        char Char = CHAR_MIN;

        while(1)
        {
            if(CharFunction(Char))
            {
                ERRORFUNCTION(GetRandomString,!AcceptableChars.Append(Char),RETURN_BOOL)
            }
            if(Char == CHAR_MAX){break;}
            Char++;
        }

        ERRORFUNCTION(GetRandomString,!AcceptableChars.IsValid(),RETURN_BOOL)
        ERRORFUNCTION(GetRandomString,!Acceptable.SetSize(AcceptableChars.GetSize()),RETURN_BOOL)
        AcceptableChars.CurrToStart();

        do{
            Acceptable[AcceptableChars.GetIter()-1] = AcceptableChars.GetCurrItem();
        }while(AcceptableChars.CurrNext());
    }


    SIZE_TYPE Temp = 0;

    while(Temp < S)
    {
        Array[Temp] = Acceptable[GetRandomInt(0,Acceptable.GetSize())];
        Temp++;
    }

    return true;
}

const TemplateArray<char> GetRandomTemplateArray(const SIZE_TYPE S, const bool (*CharFunction)(const char C))
{
    TemplateArray<char> Temp;
    ERRORFUNCTION(GetRandomTemplateArray,S < 1,return TemplateArray<char>();)
    ERRORFUNCTION(GetRandomTemplateArray,!Temp.SetSize(S),return TemplateArray<char>();)

    ERRORFUNCTION(GetRandomTemplateArray,!GetRandomString(Temp.GetArray(),S,CharFunction),return TemplateArray<char>();)
    return Temp;
}

const TemplateArray<char> GetRandomTemplateArray(const SIZE_TYPE S, int (*CharFunction)(const int C))
{
    TemplateArray<char> Temp;
    ERRORFUNCTION(GetRandomTemplateArray,S < 1,return TemplateArray<char>();)
    ERRORFUNCTION(GetRandomTemplateArray,!Temp.SetSize(S),return TemplateArray<char>();)

    ERRORFUNCTION(GetRandomTemplateArray,!GetRandomString(Temp.GetArray(),S,CharFunction),return TemplateArray<char>();)
    return Temp;
}

//This checks if the present computer's operating systen offers a random enough return to be considered 'reliable'
const bool IsComputerRandom()
{
    int Temp[20];

    SIZE_TYPE Iter = 0;

    while(Iter < 20)
    {
        Temp[Iter] = GetRandomInt();
        Iter++;
    }

    Iter = 0;
    SIZE_TYPE IsMatching = 0;
    while(Iter < 20)
    {
        Iter++;
        if(Temp[Iter-1] == Temp[Iter])
        {
            IsMatching++;
            if(IsMatching == 2)
            {
                return false;
            }
        }
        else
        {
            IsMatching = 0;
        }
    }

    return true;
}

const int GetRandomIntPath(const int Start, const int End, const CharArray Path)
{
    srand(GetRandomChar());

    if(Path.IsValid())
    {
        SIZE_TYPE Iter = 0;
        int CurrRandom = rand();

        while(Iter < Path.GetSize())
        {
            switch(Path[Iter])
            {
                case '1':
                    CurrRandom = GetRandomDividedInt(CurrRandom);
                break;
                case '2':
                    CurrRandom = GetRandomSubtractedInt(CurrRandom);
                break;
                case '3':
                    CurrRandom = GetRandomAddedInt(CurrRandom);
                break;
                case '4':
                    CurrRandom = GetRandomANDInt(CurrRandom);
                break;
                case '5':
                    CurrRandom = GetRandomMultipliedInt(CurrRandom);
                break;
                case '6':
                    CurrRandom = GetRandomORInt(CurrRandom);
                break;
                case '7':
                    CurrRandom = GetRandomXORInt(CurrRandom);
                break;
                default:
                    CurrRandom = CurrRandom+1;
                break;
            }

            Iter++;
        }
        return BetweenRange(CurrRandom,Start,End);
    }
    else
    {
        return BetweenRange(rand(),Start,End);
    }
}

}

#endif
