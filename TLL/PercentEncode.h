#ifndef G_PERCENTENCODE_H
#define G_PERCENTENCODE_H

/*
Generated on: 2012-05-25-17.24.41

Version: 1.0
*/

/*
Notes:
To be copied to RND 7

*/

/*
Suggestions for improvements:


*/

namespace TL
{

char PercentEncodeUnreservedTable[] = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM0123456789-_.~";
char PercentEncodeReservedTable[] = "!*'();:@&=+$,/#[]\\\?";
char PercentEncodeReservedTableURL[] ="!*'();@$,[]\\\?";
char PercentEncodeTableURL[] ="!*'();@$,[]\\\? ";
char PercentEncodeBase16[] = "0123456789ABCDEF";

unsigned short Base16(const char Item)
{
    if(IsCompareChar(Item,"0123456789")){ return Item-'0'; }
    if(IsCompareChar(Item,"ABCDEF")){ return (Item-'A')+10; }
    if(IsCompareChar(Item,"abcdef")){ return (Item-'a')+10; }
    return 0;
}

CharArray PercentEncodeChar(const char Letter)
{
    CharArray Temp;
    char C1 = Letter/16;
    if(!Temp.SetArray('%'))
    {
        ERRORFUNCTION(PercentEncode,1,return CharArray();)
    }

    ERRORFUNCTION(PercentEncode,!Temp.Append(PercentEncodeBase16[(int)C1]),return CharArray();)
    if(!Temp.Append(PercentEncodeBase16[(Letter%16)]))
    {
        ERRORFUNCTION(PercentEncode,1,return CharArray();)
    }
    return Temp;
}

//This assumes the array is precisely 3 long - but it does not enforce specifically 3 because it would cause errors
const char PercentDecodeChar(const char Array[])
{
    return ((Base16(Array[1])*16)+Base16(Array[2]));
}

const bool IsPercentEncoded(const char Array[])
{
    return ((Array[0] == '%') && IsCompareChar(Array[1],"0123456789ABCDEFabcdef") && IsCompareChar(Array[1],"0123456789ABCDEFabcdef"));
}

const CharArray PercentDecodeCharArray(const CharArray &ItemCopy)
{
    ERRORFUNCTION(PercentDecodeCharArray,!ItemCopy.IsValid(),return CharArray();)
    CharArray Temp;
    ERRORFUNCTION(PercentDecodeCharArray,!Temp.SetArray(ItemCopy),return CharArray();)

    const char * C_Ptr = Temp.GetArray(), * C_Ptr_2 = Temp.GetArray()+Temp.GetSize();

    SIZE_TYPE Pos = 0;
    while( ((C_Ptr = FindItem(C_Ptr,C_Ptr_2,'%')) != NULL) && C_Ptr < C_Ptr_2)
    {
        if((C_Ptr+2) > C_Ptr_2){ break; }

        if(IsPercentEncoded(C_Ptr))
        {
            Pos = C_Ptr-Temp.GetArray();
            ERRORFUNCTION(PercentDecodeCharArray,!Temp.Replace(C_Ptr,3,PercentDecodeChar(C_Ptr),Pos),return CharArray();)
            C_Ptr = (Temp.GetArray()+(Pos+1));
            C_Ptr_2 = Temp.GetArray()+Temp.GetSize();
            if(C_Ptr > C_Ptr_2){ break; }
        }
    }

    return Temp;
}

const CharArray PercentEncodeCharArray(const CharArray &URL, const CharArray &EncodeChars = PercentEncodeTableURL)
{
    ERRORFUNCTION(PercentEncodeCharArray,!URL.IsValid(),return CharArray();)
    CharArray Temp;

    ERRORFUNCTION(PercentEncodeCharArray,!Temp.SetArray(URL),return CharArray();)

    CharArray PercentEncoded;
    SIZE_TYPE Char = 0;

    while(Char != Temp.GetSize()-1)
    {
        if(FindItem(EncodeChars.GetArray(),EncodeChars.GetSize(),Temp[Char]))
        {
            ERRORFUNCTION(PercentEncodeCharArray,!PercentEncoded.SetArray(PercentEncodeChar(Temp[Char])),return CharArray();)
            ERRORFUNCTION(PercentEncodeCharArray,!Temp.Replace(Temp[Char],PercentEncoded.GetArray(),PercentEncoded.GetSize()-1,Char),return CharArray();)
            Char = Char + 2;
        }
        Char++;
    }

    return Temp;
}

}

#endif
