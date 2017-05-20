#ifndef G_MATHS_H
#define G_MATHS_H

/*
Generated on: 2012-10-17-12.06.33

Version: 1.4
*/

/*
Notes:
DO NOT UPDATE! CUSTOM VERSION!

Added limit
Added IsMultiple, which is also declared in SEG.h, so as a result, it has an unusual fail-safe around it to prevent errors

Added difference and between range functions to compensate for SFML's discontinuation of Random
Added 0 divisor safeguard on IsMultiple to prevent strange behaviour
*/

/*
Suggestions for improvements:


*/

#include <math.h>
#include <stdlib.h>

namespace TL
{

#ifndef G_SEG_H

const bool IsMultiple(const size_t Is, const size_t MultipleOf)
{
    if(Is == 0 || MultipleOf == 0)
    {
        if(Is == MultipleOf){return true;}
        return false;
    }
    float Temp = ((float)Is/(float)MultipleOf);
    Temp = Temp - (int)Temp;
    return (Temp == 0.0);
}

#endif

SIZE_TYPE Modulus(const SIZE_TYPE X, const SIZE_TYPE Y)
{
    if(X == 0){return 0;}
    return X%Y;
}

SIZE_TYPE LimitSIZETYPE(const SIZE_TYPE X, const SIZE_TYPE Y, const SIZE_TYPE Value)
{
    if(X > Y)
    {
        if(Value > X)
        {
            return X;
        }
        if(Value < Y)
        {
            return Y;
        }
    }
    else
    {
        if(Value < X)
        {
            return X;
        }
        if(Value > Y)
        {
            return Y;
        }
    }
    return Value;
}

float Limit(const float X, const float Y, const float Value)
{
    if(Value >= X && Value <= Y){return Value;}
    if(Value <= X && Value >= Y){return Value;}

    if(abs(Value-Y) < abs(Value-X))
    {
        return Y;
    }
    return X;
}

float ConvertBetweenRanges(const int X, const int Y, const int X2, const int Y2, const int Value2)
{
    if( !(Value2 <= Y2 && Value2 >= X2) && !(Value2 <= X2 && Value2 >= Y2) ){return 0.0;}

    int Range1 = abs(X2-Y2);
    int Value1 = abs(X2-Value2);
    float Amount = abs(X-Y)/(float)Range1;

    Amount = (Range1 - Value1)*Amount;

    if(X < Y){return Limit(X,Y,Y-Amount);}
    return Limit(X,Y,Y+Amount);
}

float ConvertBetweenRanges(const int X, const int Y, const float X2, const float Y2, const float Value2)
{
    if( !(Value2 <= Y2 && Value2 >= X2) && !(Value2 <= X2 && Value2 >= Y2) ){return 0.0;}

    float Range1 = fabs(X2-Y2);
    float Value1 = fabs(X2-Value2);
    float Amount = abs(X-Y)/(float)Range1;

    Amount = (Range1 - Value1)*Amount;

    if(X < Y){return Limit(X,Y,Y-Amount);}
    return Limit(X,Y,Y+Amount);
}

float Round(float X)
{
    return (X > 0.0) ? floor(X + 0.5) : ceil(X - 0.5);
}

double Round(double X)
{
    return (X > 0.0) ? floor(X + 0.5) : ceil(X - 0.5);
}

//Whilst casting to int truncates the numbers after the .
//this will round up or down then truncate to int
//Also, it's statically cast and avoids messy duplicate code of
//static_cast<Round(X)> or (int)Round(X) which can be confusing
int RoundToInt(float X)
{
    return static_cast<int>(Round(X));
}

const long Difference(const int X, const int Y)
{
    return abs(X-Y);
}

const float DifferenceFloat(const float X, const float Y)
{
    return fabs(X-Y);
}

const bool IsEven(const int X)
{
    float Temp = X;
    Temp = Temp/2;
    Temp = Temp - (int)Temp;
    return (Temp == 0.0);
}

const int BetweenRange(const int Value, const int X, const int Y)
{
    return X+(abs(Value)%(Difference(X,Y)+1));
}

const unsigned int BetweenRangeUnsignedInt(const unsigned int Value, const unsigned int X, const unsigned int Y)
{
    return X+(abs(Value)%(Difference(X,Y)+1));
}

const float BetweenRangeFloat(const float Value, const float X, const float Y)
{
    return X+(fmod(fabs(Value),(DifferenceFloat(X,Y))+1));
}

const bool IsOdd(const int X)
{
    return !IsEven(X);
}

}

#endif
