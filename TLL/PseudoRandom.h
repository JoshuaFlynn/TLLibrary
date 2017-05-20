#ifndef G_PSEUDORANDOM_H
#define G_PSEUDORANDOM_H

/*
Generated on: 2013-12-01-18.19.16

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
#include <climits>
#include <cctype>

#ifndef MAX_RANDOM
#define MAX_RANDOM 200
#endif

namespace TL
{

namespace PR
{
int G_PseudoRandomIter = 0;
}

const bool GetPseudoRandomBool(const int Total)
{
    if(PR::G_PseudoRandomIter > Total)
    {
        PR::G_PseudoRandomIter = 0;
    }
    else if(PR::G_PseudoRandomIter < 0)
    {
        PR::G_PseudoRandomIter = 0;
    }

    TemplateArray<int> Pseudo;

    if(Total < 1)
    {
        return false;
    }

    if(!Pseudo.SetSize(Total))
    {
        return true;
    }

    int Random = Pseudo[(PR::G_PseudoRandomIter%Total)];
    Random = (int)&Pseudo[(PR::G_PseudoRandomIter%Total)] ;

    PR::G_PseudoRandomIter++;
    if(Random == 0){Random = PR::G_PseudoRandomIter;}

    return ((Random&1) == 1 || (Random&2) == 2);
}

const int GetPseudoRandomInt(const int Total)
{
    int Temp = 0;

    for(unsigned int I = 0;I < (sizeof(int)*8);I++)
    {
        Temp = Temp + (GetPseudoRandomBool(Total) << I);
    }
    return Temp;
}

const unsigned int GetPseudoRandomUnsignedInt(const int Total)
{
    unsigned int Temp = 0;

    for(unsigned int I = 0;I < (sizeof(unsigned int)*8);I++)
    {
        Temp = Temp + (GetPseudoRandomBool(Total) << I);
    }
    return Temp;
}

const short GetPseudoRandomShort(const int Total)
{
    short Temp = 0;

    for(unsigned int I = 0;I < (sizeof(short)*8);I++)
    {
        Temp = Temp + (GetPseudoRandomBool(Total) << I);
    }

    return Temp;
}

const unsigned short GetPseudoRandomUnsignedShort(const int Total)
{
    unsigned short Temp = 0;

    for(unsigned int I = 0;I < (sizeof(unsigned short)*8);I++)
    {
        Temp = Temp + (GetPseudoRandomBool(Total) << I);
    }

    return Temp;
}

const char GetPseudoRandomChar(const int Total)
{
    char Temp = 0;

    for(unsigned int I = 0;I < (sizeof(char)*8);I++)
    {
        Temp = Temp + (GetPseudoRandomBool(Total) << I);
    }

    return Temp;
}

const int GetPseudoRandomInt(const int Start, const int End, const int Total)
{
    int Temp = abs(Start-End);
    if(Temp == 0){return 0;}
    Temp++;//To include the actual number
    if(Start > End)
    {
        return (End+(GetPseudoRandomUnsignedInt(Total)%Temp));
    }
    else
    {
        return (Start+(GetPseudoRandomUnsignedInt(Total)%Temp));
    }
}

const bool GetPseudoRandomString(char Array[], const SIZE_TYPE S, const char CharStart, const char CharEnd, bool (*CharFunction)(const char C), const int Total)
{
    ERRORFUNCTION(GetPseudoRandomString,!Array,RETURN_BOOL)
    ERRORFUNCTION(GetPseudoRandomString,S < 1,RETURN_BOOL)

    unsigned int Temp = 0;
    char C = 0;
    while(Temp < S)
    {
        C = GetPseudoRandomInt(CharStart,CharEnd,Total);
        if(CharFunction(C) > 0)
        {
            Array[Temp] = C;
            Temp++;
        }
    }
    return true;
}

const bool GetPseudoRandomString(char Array[], const SIZE_TYPE S, const int CharStart, const int CharEnd, int (*CharFunction)(const int C), const int Total)
{
    ERRORFUNCTION(GetPseudoRandomString,!Array,RETURN_BOOL)
    ERRORFUNCTION(GetPseudoRandomString,S < 1,RETURN_BOOL)

    unsigned int Temp = 0;
    int C = 0;
    while(Temp < S)
    {
        C = GetPseudoRandomInt(CharStart,CharEnd,Total);
        if(CharFunction(C) > 0)
        {
            Array[Temp] = C;
            Temp++;
        }
    }
    return true;
}

const bool GetPseudoRandomString(char Array[], const SIZE_TYPE S, const bool (*CharFunction)(const char C), const int Total)
{
    ERRORFUNCTION(GetPseudoRandomString,!Array,RETURN_BOOL)
    ERRORFUNCTION(GetPseudoRandomString,S < 1,RETURN_BOOL)

    unsigned int Temp = 0;
    char C = 0;
    while(Temp < S)
    {
        C = GetPseudoRandomChar(Total);
        if(CharFunction(C) > 0)
        {
            Array[Temp] = C;
            Temp++;
        }
    }
    return true;
}

//This accepts a point to a char function to define what type of characters you want in the random string
//This is the int version, designed for compatibility with C type functions
const bool GetPseudoRandomString(char Array[], const SIZE_TYPE S, int (*CharFunction)(const int C),const int Total)
{
    ERRORFUNCTION(GetPseudoRandomString,!Array,RETURN_BOOL)
    ERRORFUNCTION(GetPseudoRandomString,S < 1,RETURN_BOOL)

    SIZE_TYPE Temp = 0;
    int C = 0;

    while(Temp < S)
    {
        C = GetPseudoRandomInt(CHAR_MIN,CHAR_MAX,Total);
        if(CharFunction(C) > 0)
        {
            Array[Temp] = C;
            Temp++;
        }
    }
    return true;
}

const TemplateArray<char> GetPseudoRandomTemplateArray(const SIZE_TYPE S, const bool (*CharFunction)(const char C), const int Total)
{
    TemplateArray<char> Temp;
    ERRORFUNCTION(GetPseudoRandomTemplateArray,S < 1,return TemplateArray<char>();)
    ERRORFUNCTION(GetPseudoRandomTemplateArray,!Temp.SetSize(S),return TemplateArray<char>();)

    ERRORFUNCTION(GetPseudoRandomTemplateArray,!GetPseudoRandomString(Temp.GetArray(),S,CharFunction,Total),return TemplateArray<char>();)
    return Temp;
}

const TemplateArray<char> GetPseudoRandomTemplateArray(const SIZE_TYPE S, int (*CharFunction)(const int C), const int Total)
{
    TemplateArray<char> Temp;
    ERRORFUNCTION(GetPseudoRandomTemplateArray,S < 1,return TemplateArray<char>();)
    ERRORFUNCTION(GetPseudoRandomTemplateArray,!Temp.SetSize(S),return TemplateArray<char>();)

    ERRORFUNCTION(GetPseudoRandomTemplateArray,!GetPseudoRandomString(Temp.GetArray(),S,CharFunction,Total),return TemplateArray<char>();)
    return Temp;
}

const bool GetPseudoRandomBool()
{
    if(PR::G_PseudoRandomIter > MAX_RANDOM)
    {
        PR::G_PseudoRandomIter = 0;
    }
    else if(PR::G_PseudoRandomIter < 0)
    {
        PR::G_PseudoRandomIter = 0;
    }

    TemplateArray<int> Pseudo;

    if(MAX_RANDOM < 1)
    {
        return false;
    }

    if(!Pseudo.SetSize(MAX_RANDOM))
    {
        return true;
    }

    int Random = Pseudo[(PR::G_PseudoRandomIter%MAX_RANDOM)] + (int)&Pseudo[(PR::G_PseudoRandomIter%MAX_RANDOM)];
    PR::G_PseudoRandomIter++;
    if(Random == 0){Random = PR::G_PseudoRandomIter;}

    return ((Random&1) == 1 || (Random&2) == 2);
}

const int GetPseudoRandomInt()
{
    int Temp = 0;

    for(unsigned int I = 0;I < (sizeof(int)*8);I++)
    {
        Temp = Temp + (GetPseudoRandomBool() << I);
    }
    return Temp;
}

const unsigned int GetPseudoRandomUnsignedInt()
{
    unsigned int Temp = 0;

    for(unsigned int I = 0;I < (sizeof(unsigned int)*8);I++)
    {
        Temp = Temp + (GetPseudoRandomBool() << I);
    }
    return Temp;
}

const short GetPseudoRandomShort()
{
    short Temp = 0;

    for(unsigned int I = 0;I < (sizeof(short)*8);I++)
    {
        Temp = Temp + (GetPseudoRandomBool() << I);
    }

    return Temp;
}

const unsigned short GetPseudoRandomUnsignedShort()
{
    unsigned short Temp = 0;

    for(unsigned int I = 0;I < (sizeof(unsigned short)*8);I++)
    {
        Temp = Temp + (GetPseudoRandomBool() << I);
    }

    return Temp;
}

const int GetPseudoRandomXORInt(const int X = GetPseudoRandomInt())
{
    return GetPseudoRandomInt() ^ X;
}

const int GetPseudoRandomAddedInt(const int X = GetPseudoRandomInt())
{
    return GetPseudoRandomInt() + X;
}

const int GetPseudoRandomSubtractedInt(const int X = GetPseudoRandomInt())
{
    return GetPseudoRandomInt() - X;
}

const int GetPseudoRandomMultipliedInt(const int X = GetPseudoRandomInt())
{
    int Y = GetPseudoRandomInt();
    if(X == 0){return 0;}
    if(Y == 0){return 0;}
    return X*Y;
}

const int GetPseudoRandomDividedInt(const int X = GetPseudoRandomInt())
{
    int Y = GetPseudoRandomInt();
    if(X == 0){return 0;}
    if(Y == 0){return 0;}
    return X/Y;
}

const int GetPseudoRandomANDInt(const int X = GetPseudoRandomInt())
{
    return (GetPseudoRandomInt())&X;
}

const int GetPseudoRandomORInt(const int X = GetPseudoRandomInt())
{
    return (GetPseudoRandomInt())|X;
}

const char GetPseudoRandomChar()
{
    char Temp = 0;

    for(unsigned int I = 0;I < (sizeof(char)*8);I++)
    {
        Temp = Temp + (GetPseudoRandomBool() << I);
    }

    return Temp;
}

const int GetPseudoRandomInt(const int Start, const int End)
{
    int Temp = abs(Start-End);
    if(Temp == 0){return 0;}
    Temp++;//To include the actual number
    if(Start > End)
    {
        return (End+(GetPseudoRandomUnsignedInt()%Temp));
    }
    else
    {
        return (Start+(GetPseudoRandomUnsignedInt()%Temp));
    }
}

const int GetPseudoRandomAverage(const int Start, const int End, const unsigned int Averages)
{
    if(Averages == 0){return 0;}
    int Temp = 0;
    for(unsigned int I = 0; I < Averages;I++)
    {
        Temp = Temp + GetPseudoRandomInt(Start,End);
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


const bool GetPseudoRandomString(char Array[], const SIZE_TYPE S, const char CharStart, const char CharEnd, bool (*CharFunction)(const char C))
{
    ERRORFUNCTION(GetPseudoRandomString,!Array,RETURN_BOOL)
    ERRORFUNCTION(GetPseudoRandomString,S < 1,RETURN_BOOL)

    unsigned int Temp = 0;
    char C = 0;
    while(Temp < S)
    {
        C = GetPseudoRandomInt(CharStart,CharEnd,S+S);
        if(CharFunction(C) > 0)
        {
            Array[Temp] = C;
            Temp++;
        }
    }
    return true;
}

const bool GetPseudoRandomString(char Array[], const SIZE_TYPE S, const int CharStart, const int CharEnd, int (*CharFunction)(const int C))
{
    ERRORFUNCTION(GetPseudoRandomString,!Array,RETURN_BOOL)
    ERRORFUNCTION(GetPseudoRandomString,S < 1,RETURN_BOOL)

    unsigned int Temp = 0;
    int C = 0;
    while(Temp < S)
    {
        C = GetPseudoRandomInt(CharStart,CharEnd,S+S);
        if(CharFunction(C) > 0)
        {
            Array[Temp] = C;
            Temp++;
        }
    }
    return true;
}

const bool GetPseudoRandomString(char Array[], const SIZE_TYPE S, const bool (*CharFunction)(const char C))
{
    ERRORFUNCTION(GetPseudoRandomString,!Array,RETURN_BOOL)
    ERRORFUNCTION(GetPseudoRandomString,S < 1,RETURN_BOOL)

    unsigned int Temp = 0;
    char C = 0;
    while(Temp < S)
    {
        C = GetPseudoRandomChar(S+S);
        if(CharFunction(C) > 0)
        {
            Array[Temp] = C;
            Temp++;
        }
    }
    return true;
}

//This accepts a point to a char function to define what type of characters you want in the random string
//This is the int version, designed for compatibility with C type functions
const bool GetPseudoRandomString(char Array[], const SIZE_TYPE S, int (*CharFunction)(const int C))
{
    ERRORFUNCTION(GetPseudoRandomString,!Array,RETURN_BOOL)
    ERRORFUNCTION(GetPseudoRandomString,S < 1,RETURN_BOOL)

    SIZE_TYPE Temp = 0;
    int C = 0;

    while(Temp < S)
    {
        C = GetPseudoRandomInt(CHAR_MIN,CHAR_MAX,S+S);
        if(CharFunction(C) > 0)
        {
            Array[Temp] = C;
            Temp++;
        }
    }
    return true;
}

const TemplateArray<char> GetPseudoRandomTemplateArray(const SIZE_TYPE S, const bool (*CharFunction)(const char C))
{
    TemplateArray<char> Temp;
    ERRORFUNCTION(GetPseudoRandomTemplateArray,S < 1,return TemplateArray<char>();)
    ERRORFUNCTION(GetPseudoRandomTemplateArray,!Temp.SetSize(S),return TemplateArray<char>();)

    ERRORFUNCTION(GetPseudoRandomTemplateArray,!GetPseudoRandomString(Temp.GetArray(),S,CharFunction,S+S),return TemplateArray<char>();)
    return Temp;
}

const TemplateArray<char> GetPseudoRandomTemplateArray(const SIZE_TYPE S, int (*CharFunction)(const int C))
{
    TemplateArray<char> Temp;
    ERRORFUNCTION(GetPseudoRandomTemplateArray,S < 1,return TemplateArray<char>();)
    ERRORFUNCTION(GetPseudoRandomTemplateArray,!Temp.SetSize(S),return TemplateArray<char>();)

    ERRORFUNCTION(GetPseudoRandomTemplateArray,!GetPseudoRandomString(Temp.GetArray(),S,CharFunction,S+S),return TemplateArray<char>();)
    return Temp;
}

}


#undef MAX_RANDOM
#endif
