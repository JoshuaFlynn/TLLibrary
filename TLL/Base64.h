#ifndef G_BASE64_H
#define G_BASE64_H

/*
Generated on: 2012-04-29-12.44.16

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:
The functions shouldn't be based on a class, but just general functions.
Decoder can make use of the 'RemoveStringRepeat' function for more efficient removal

*/

#include "FileProcEquals.h"

#include <inttypes.h>
#include "CharArray.h"
#include "TemplateArray.h"
#include "TemplateList.h"

#define MIME_SPACING "\r\n"

namespace TL
{

char Base64Table[] =
{
    'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
    '0','1','2','3','4','5','6','7','8','9','+','/'
};

//76 for MIME
//64 for PEM - SMTP uses PEM
class Base64
{
	private:
	protected:

        const char GetEncodeChar(const char Char) const { return Base64Table[(unsigned)Char]; }

        class Base64Byte
        {
            protected:

                //Assumes character is an encoded type
                const char GetDecodeChar(const char Char) const
                {
                    if((Char >= 'A') && (Char <= 'Z')){ return (Char-65); }
                    if((Char >= 'a') && (Char <= 'z')){ return (Char-71); }
                    if( (Char >= '0') && (Char <= '9') ){ return (Char+4);}
                    if(Char == '+'){return 62;}
                    if(Char == '/'){return 63;}
                    //We return only 0 because it's either = or an invalid character
                    return 0;
                }

            public:
                uint8_t Char3;
                uint8_t Char2;
                uint8_t Char1;

                void ProcessChars(const char Array[])
                {
                    uint32_t *Ptr = (uint32_t *)this;
                    *Ptr = (GetDecodeChar(Array[0]) << 3 * 6) + (GetDecodeChar(Array[1]) << 2 * 6) + (GetDecodeChar(Array[2]) << 1 * 6) + (GetDecodeChar(Array[3]) << 0 * 6);
                    return;
                }
        };

        const unsigned char PowerOf3(const TemplateArray<char> &Array) const { return (Array.GetSize()%3); }
        const unsigned char PowerOf3(const SIZE_TYPE I) const { return (I%3); }
        const unsigned char PowerOf4(const TemplateArray<char> &Array) const { return (Array.GetSize()%4); }
        const unsigned char PowerOf4(const SIZE_TYPE I) const { return (I%4); }

        const bool IsPowerOf3(const TemplateArray<char> &Array) const { return (PowerOf3(Array) == 0); }
        const bool IsPowerOf4(const TemplateArray<char> &Array) const { return (PowerOf4(Array) == 0); }

        void AddChar(TemplateArray<char> &Temp, SIZE_TYPE &Iter, const SIZE_TYPE &Offset, const char ItemCopy, const SIZE_TYPE &TempIter, const SIZE_TYPE &NewlineSpacing)
        {
            if(NewlineSpacing != 0)
            {
                if((TempIter+Offset)%NewlineSpacing == 0)
                {
                    Temp.OverWrite(Iter+Offset,MIME_SPACING,strlen(MIME_SPACING));
                    Iter = Iter + 2;
                }
            }
            Temp[Iter+Offset] = ItemCopy;
        }

	public:
		Base64(){}
		~Base64(){}

        const TemplateArray<char> Encode(const char Array[], const SIZE_TYPE S, const SIZE_TYPE NewlineSpacing = 0)
        {
            char T = (S%3);

            if(T == 2){ T = 1; }
            else
            {
                if(T == 1){ T = 2; }
            }

            SIZE_TYPE NumberOfBase64Bytes = ((S/3)*4)+(4*(T != 0));
            TemplateArray<char> Temp;

            Temp.SetSize(NumberOfBase64Bytes);

            Base64Byte Item;
            uint32_t *Info = (uint32_t *)&Item;

            SIZE_TYPE Iter = 0;
            SIZE_TYPE ArrayIter = 0;

            while(ArrayIter < S)
            {
                Item.Char1 = Array[ArrayIter+0];
                Item.Char2 = Array[ArrayIter+1];
                Item.Char3 = Array[ArrayIter+2];

                Temp[Iter+0] = GetEncodeChar(*Info >> 18 & 63);
                Temp[Iter+1] = GetEncodeChar(*Info >> 12 & 63);

                if((Iter+4) == Temp.GetSize())
                {
                    if(T == 0)
                    {
                        Temp[Iter+2] = GetEncodeChar(*Info >> 6 & 63);
                        Temp[Iter+3] = GetEncodeChar(*Info >> 0 & 63);
                    }

                    if(T == 1)
                    {
                        Temp[Iter+2] = GetEncodeChar(*Info >> 6 & 63);
                        Temp[Iter+3] = '=';
                    }

                    if(T == 2)
                    {
                        Temp[Iter+2] = '=';
                        Temp[Iter+3] = '=';
                    }
                }
                else
                {
                    Temp[Iter+2] = GetEncodeChar(*Info >> 6 & 63);
                    Temp[Iter+3] = GetEncodeChar(*Info >> 0 & 63);
                }

                Iter = Iter + 4;
                ArrayIter = ArrayIter + 3;
            }

            if(NewlineSpacing > 0)
            {
                TemplateArray<char> Temp3;
                ERRORCLASSFUNCTION(Base64,Encode,!Temp3.SetSize(Temp.GetSize()+(strlen(MIME_SPACING)*(Temp.GetSize()/NewlineSpacing))),return TemplateArray<char>();)
                ERRORCLASSFUNCTION(Base64,Encode,!InsertStringRepeat(Temp3.GetArray(),Temp.GetArray(),Temp.GetSize(),MIME_SPACING,strlen(MIME_SPACING),NewlineSpacing),return TemplateArray<char>(); )
                return Temp3;
            }
            return Temp;
        }


        const TemplateArray<char> Encode(const CharArray &Data, const SIZE_TYPE NewlineSpacing)
        {
            ERRORCLASSFUNCTION(Base64,Encode,!Data.IsValid(),return TemplateArray<char>();)
            //Adapts for null on the end
            return Encode(Data.GetArray(),Data.GetSize()-1,NewlineSpacing);
        }

        const TemplateArray<char> Encode(const TemplateArray<char> &Data, const SIZE_TYPE NewlineSpacing)
        {
            ERRORCLASSFUNCTION(Base64,Encode,!Data.IsValid(),return TemplateArray<char>();)
            return Encode(Data.GetArray(),Data.GetSize(),NewlineSpacing);
        }

        const TemplateArray<char> Decode(const char Array[], const SIZE_TYPE S)
        {
            ERRORCLASSFUNCTION(Base64,Decode,!Array,return TemplateArray<char>();)
            ERRORCLASSFUNCTION(Base64,Decode,S < 0,return TemplateArray<char>();)

            TemplateArray<char> Temp;
            ERRORCLASSFUNCTION(Base64,Decode,!Temp.SetArray(Array,S),return TemplateArray<char>();)

            while(Find(Temp.GetArray(),Temp.GetSize(),MIME_SPACING,strlen(MIME_SPACING)))
            {
                ERRORCLASSFUNCTION(Base64,Decode,!Temp.Remove(MIME_SPACING,strlen(MIME_SPACING)),return TemplateArray<char>();)
            }

            while(FindItemBackwards(Temp.GetArray(),Temp.GetSize(),'='))
            {
                ERRORCLASSFUNCTION(Base64,Decode,!Temp.Replace('=','\0'),return TemplateArray<char>();)
            }

            ERRORCLASSFUNCTION(Base64,Decode,!IsPowerOf4(Temp),return TemplateArray<char>();)

            short Number = 0;

            if(Find(Array,S,"==",2,false))
            {
                Number = 2;
            }
            else
            {
                if(FindItemBackwards(Array,S,'='))
                {
                    Number = 1;
                }
            }

            TemplateArray<char> Temp2;
            ERRORCLASSFUNCTION(Base64,Decode,!Temp2.SetSize( (3*(Temp.GetSize()/4))-Number ),return TemplateArray<char>();)

            Base64Byte Ptr;

            SIZE_TYPE Iter3 = 0;
            SIZE_TYPE Iter2 = 0;
            while(Iter3 < Temp.GetSize())
            {
                Ptr.ProcessChars(&Temp[Iter3]);

                Temp2[Iter2] = Ptr.Char1;
                if( (Iter3+4) == Temp.GetSize() )
                {
                    if(Number == 2){break;}
                    if(Number == 1)
                    {
                        Temp2[Iter2+1] = Ptr.Char2;
                        break;
                    }
                }

                Temp2[Iter2+1] = Ptr.Char2;
                Temp2[Iter2+2] = Ptr.Char3;

                Iter3 = Iter3 + 4;
                Iter2 = Iter2 + 3;
            }

            return Temp2;
        }

        const TemplateArray<char> Decode(const CharArray &Data)
        {
            ERRORCLASSFUNCTION(Base64,Decode,!Data.IsValid(),return TemplateArray<char>();)
            //Adapts for null on the end
            return Decode(Data.GetArray(),Data.GetSize()-1);
        }

        const TemplateArray<char> Decode(const TemplateArray<char> &Data)
        {
            ERRORCLASSFUNCTION(Base64,Decode,!Data.IsValid(),return TemplateArray<char>();)
            return Decode(Data.GetArray(),Data.GetSize());
        }
};

}

#endif
