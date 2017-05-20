#ifndef G_FLAGOPS_H
#define G_FLAGOPS_H

/*
Generated on: 2012-04-02-13.13.11

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:

Better naming conventions.
Probably a good idea to indicate what exactly is meant by a 'flag'.
In this case it means an individual bit. BitFlags enabled?

*/

#include "ErrorMessage.h"
#include <stdio.h>
#include <stdint.h>

template<typename TItem, typename TItem2>
const bool AreFlagsEnabled(const TItem &BitFlags, const TItem2 &Flags)
{
    if((BitFlags&Flags) == Flags)
    {
        return true;
    }
    return false;
}

template<typename TItem, typename TItem2>
const bool AreFlagsDisabled(const TItem &BitFlags, const TItem2 &Flags)
{
    if((BitFlags&Flags) != 0)
    {
        return false;
    }
    return true;
}

template<typename TItem, typename TItem2>
void SetEnableFlags(TItem &BitFlags, const TItem2 &Flags)
{
    BitFlags = (BitFlags|Flags);
}

template<typename TItem, typename TItem2>
const TItem &EnableFlags(const TItem &BitFlags, const TItem2 &Flags)
{
    return (BitFlags|Flags);
}

template<typename TItem, typename TItem2>
void SetDisableFlags(TItem &BitFlags, const TItem2 &Flags)
{
    BitFlags = (BitFlags&(~Flags));
}

template<typename TItem, typename TItem2>
const TItem &DisableFlags(const TItem &BitFlags, const TItem2 &Flags)
{
    return (BitFlags&(~Flags));
}

template<typename TItem>
const TItem &MoveFlagBack(const TItem &Flags, const unsigned int Places)
{
    return (Flags>>Places);
}

const unsigned int GetConvertFlag(const unsigned int Flag)
{
    return (1<<Flag);
}

template<typename TItem>
const TItem GetNumberOfBits(const TItem &Flags, const unsigned short Bits, const unsigned short OffSet)
{
    TItem FlagsReturn = 0;
    int I = 0;
    while(I < Bits)
    {
        FlagsReturn = (FlagsReturn | ((TItem)GetConvertFlag(I)*AreFlagsEnabled(Flags,GetConvertFlag(OffSet+I))));
        I++;
    }
    return FlagsReturn;
}

//Designed to push bits onto a byte for file writing operations
//It pushes onto Target from left-to-right, and counts from right-to-left on Bits when counting how many to push
//So 10100000 on Target, with only 00000111 on Bits, would result in 10111100, as though pushed to the 'end' (the end being left)
const bool Push8Bits(uint8_t &Target, const uint8_t BitsAlreadyOnStack, const uint8_t NumberOfBitsToPush, const uint8_t Bits)
{
    ERRORFUNCTION(Push8Bits,BitsAlreadyOnStack > 8,RETURN_BOOL)
    ERRORFUNCTION(Push8Bits,NumberOfBitsToPush < 1,RETURN_BOOL)
    ERRORFUNCTION(Push8Bits,(NumberOfBitsToPush+BitsAlreadyOnStack) > 8,RETURN_BOOL)

    Target = Target | (Bits<<(8-BitsAlreadyOnStack));
    //I put brackets around -1, undo this if there are any problems
    Target = Target & ( ((uint8_t) -1 ) << 8 - (BitsAlreadyOnStack+NumberOfBitsToPush)); //Removes unwanted bits
    return true;
}

//This doesn't actually extract 8 bits, but it's an extraction system designed for an 8-bit byte
//which can extract bits smaller than this (so not to confuse with '16bit' or '32bit' integers
//const bool Extract8Bits(uint8_t &Target, const uint8_t NumberOfBitsToExtract, )

const bool ExtractBits(uint8_t &Target, const uint8_t LTRO, const uint8_t BTR, const uint8_t Byte)
{
    if(LTRO > 7 || LTRO < 0){return false;}
    if(BTR > 8 || BTR < 1){return false;}
    if((LTRO + BTR) > 8){return false;}

    Target = Byte<<LTRO;
    Target = Target>>LTRO;
    Target = Target>>(8-(LTRO+BTR));
    return true;
}

const bool PushBits(uint16_t &TargetByte, const uint8_t Length, const uint8_t Bits, const uint8_t Length2)
{
    if(Length > 16 || Length < 0){return false;}
    if( (Length+Length2) > 16){return false;}
    if(Length2 > 8 || Length < 1){return false;}
    TargetByte = TargetByte << Length2;
    uint8_t Byte = (Bits<<(8-Length2));
    Byte = Byte>>(8-Length2);
    TargetByte = TargetByte | Byte;
    return true;
}

#endif
