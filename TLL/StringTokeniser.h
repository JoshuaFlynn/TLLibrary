#ifndef G_STRINGTOKENISER_H
#define G_STRINGTOKENISER_H

/*
Generated on: 2011-11-02-11.08.22

Version: 3.2
*/

/*
Notes:
Swap fixed

Duplicate transfer from removed, but swap added

Upgraded 'RemoveString' to be reliable in the event of a duplicate string

Added some additional errno = EIO; support

Added 'splitonliteral' option to 'Tokenise' string to fix unexpected behaviours

Updated CopyStringTokeniser to SetStringTokeniser so it conforms with the TL standard definitions
*/

/*
Suggestions for improvements:

ScanRight, ScanLeft operate even on a non-tokenised string.
*/

#include <stdio.h>
#include <iostream>
#include "CommonFunctions.h"
#include "CharArray.h"

#ifndef END_VALUE
#define END_VALUE '\0'
#endif

/*
String tokeniser takes a string of characters,
and using basic C string style techniques,
turns it into a set of strings (although as one unified string)
offering iterational support, backwards, forwards, size of, numbers of etc
//Offering literal string characters support

*/

namespace TL
{

class StringTokeniser
{
    protected:
        CharArray String;
        CharArray SplitChars;
        CharArray LiteralChars;

        char * C_Ptr;
        SIZE_TYPE TokenNumber;
        SIZE_TYPE NumberOfTokens;

        void Clear(){C_Ptr = NULL; NumberOfTokens = 0; TokenNumber = 0;}
        //This function assumes it will never be called on an invalid string
        //and therefore does not return a value
        //If this is called for an invalid string, there is something wrong with the calling function
        //the calling function should only call this if the string is valid, or quit out if it's not
        void CountNumberOfTokens()
        {
            ERRORCLASSFUNCTION(StringTokeniser,CountNumberOfTokens,!String.IsValid(),NumberOfTokens = 0; RETURN_VOID)
            const char *C_Ptr_2 = String.GetArray();
            SIZE_TYPE TempSize = 0;

            NumberOfTokens = 0;
            bool PassedNull = false;
            while(TempSize != String.GetSize())
            {
                if(!PassedNull)
                {
                    if(C_Ptr_2[TempSize] != END_VALUE)
                    {
                        PassedNull = true;
                        NumberOfTokens++;
                    }
                }
                else
                {
                    if(C_Ptr_2[TempSize] == END_VALUE)
                    {
                        PassedNull = false;
                    }
                }
                ++TempSize;
            }

            return;
        }
    public:
        
        void Swap(StringTokeniser &ItemCopy)
        {
            char * C_Ptr2 = C_Ptr;
            C_Ptr = ItemCopy.C_Ptr;
            ItemCopy.C_Ptr = C_Ptr2;
            
            
            SIZE_TYPE TokenNumber2 = TokenNumber;
            SIZE_TYPE NumberOfTokens2 = NumberOfTokens;
            
            TokenNumber = ItemCopy.TokenNumber;
            ItemCopy.TokenNumber = TokenNumber2;
            
            NumberOfTokens = ItemCopy.NumberOfTokens;
            ItemCopy.NumberOfTokens = NumberOfTokens2;
            
            String.Swap(ItemCopy.String);
            SplitChars.Swap(ItemCopy.SplitChars);
            LiteralChars.Swap(ItemCopy.LiteralChars);
            return;
        }

        void Open()
        {
            Clear();
            ERRORCLASSFUNCTION(StringTokeniser,Open,!String.SetArray(""),errno = EIO;)
            ERRORCLASSFUNCTION(StringTokeniser,Open,!SplitChars.SetArray(""),errno = EIO;)
            ERRORCLASSFUNCTION(StringTokeniser,Open,!LiteralChars.SetArray(""),errno = EIO;)
        }
        void Close()
        {
            String.Close();
            SplitChars.Close();
            LiteralChars.Close();
            Clear();
        }

        void Reset()
        {
            Close();
            Open();
        }

        StringTokeniser(const StringTokeniser &ItemCopy)
        {
            ERRORCLASSFUNCTION(StringTokeniser,StringTokeniser,!SetStringTokeniser(ItemCopy),)
            return;
        }

        StringTokeniser(const CharArray &TempString, const CharArray &TempSplitChars = "", const CharArray &TempLiteralChars = "", const bool TokeniseStringBool = false)
        {
            Open();
            ERRORCLASSFUNCTION(StringTokeniser,StringTokeniser,!SetString(TempString),errno = EIO;)
            ERRORCLASSFUNCTION(StringTokeniser,StringTokeniser,!SetSplitChars(TempSplitChars),errno = EIO;)
            ERRORCLASSFUNCTION(StringTokeniser,StringTokeniser,!SetLiteralChars(TempLiteralChars),errno = EIO;)
            if(TokeniseStringBool)
            {
                ERRORCLASSFUNCTION(StringTokeniser,StringTokeniser,!TokeniseString(),)
            }
        }
        StringTokeniser(){Open();}
        ~StringTokeniser(){Close();}

        const bool IsValid() const
        {
            return (String.IsValid()&&SplitChars.IsValid()&&LiteralChars.IsValid()&&TokenNumber&&NumberOfTokens&&C_Ptr);
        }

        const bool IsEmpty() const
        {
            return (String.IsEmpty()&&SplitChars.IsEmpty()&&LiteralChars.IsEmpty()&&!TokenNumber&&!NumberOfTokens&&!C_Ptr);
        }

        const bool operator!() const {return IsEmpty();}
        operator const bool() const{return IsValid();}

        const CharArray &GetString() const {return String;}
        const CharArray &GetSplitChars() const {return SplitChars;}
        const CharArray &GetLiteralChars() const {return LiteralChars;}

        const std::string ToString() const
        {
            return TL::ToString(C_Ptr,Length(C_Ptr,END_VALUE));
        }

        const std::vector<char> ToVector() const
        {
            return TL::ToVector(C_Ptr,Length(C_Ptr,END_VALUE));
        }

        inline const char *GetCurrString() const {return C_Ptr;}
        inline const SIZE_TYPE &GetTokenNumber() const {return TokenNumber;}

        inline const SIZE_TYPE &GetNumberOfTokens() const
        {
            return NumberOfTokens;
        }

        inline const bool SetString(const CharArray &ItemCopy)
        {
            ERRORCLASSFUNCTION(StringTokeniser,SetString,!String.SetArray(ItemCopy),RETURN_BOOL)
            NumberOfTokens = 0;
            TokenNumber = 0;
            C_Ptr = String.GetArray();
            CountNumberOfTokens();
            return true;
        }

        inline const bool SetSplitChars(const CharArray &ItemCopy)
        {
            ERRORCLASSFUNCTION(StringTokeniser,SetString,!SplitChars.SetArray(ItemCopy),RETURN_BOOL)
            return true;
        }

        inline const bool SetLiteralChars(const CharArray &ItemCopy)
        {
            ERRORCLASSFUNCTION(StringTokeniser,SetString,!LiteralChars.SetArray(ItemCopy),RETURN_BOOL)
            return true;
        }

        inline const bool ScanToFirstToken()
        {
            ERRORCLASSFUNCTION(StringTokeniser,ScanToFirstToken,!String.IsValid(),RETURN_BOOL)
            C_Ptr = String.GetArray();
            SIZE_TYPE TempSize = 0;

            while(TempSize < String.GetSize())
            {
                if(C_Ptr[TempSize] != END_VALUE)
                {
                    C_Ptr = &C_Ptr[TempSize];
                    TokenNumber = 1;
                    return true;
                }
                ++TempSize;
            }
            return false;
        }

        inline const bool ScanToLastToken()
        {
            ERRORCLASSFUNCTION(StringTokeniser,ScanToFirstToken,!String.IsValid(),RETURN_BOOL)
            C_Ptr = String.GetArray();
            SIZE_TYPE TempSize = String.GetSize()-1;
            bool PassedNull = false;
            while(TempSize != 0)
            {
                if(!PassedNull)
                {
                    if(C_Ptr[TempSize] != END_VALUE)
                    {
                        PassedNull = true;
                    }
                }
                else
                {
                    if(C_Ptr[TempSize] == END_VALUE)
                    {
                        TempSize++;
                        C_Ptr = &C_Ptr[TempSize];
                        TokenNumber = GetNumberOfTokens();
                        return true;
                    }
                }

                --TempSize;
            }
            if(PassedNull)
            {
                //TokenNumber = GetTokenNumber();
                C_Ptr = &C_Ptr[TempSize];
                TokenNumber = GetNumberOfTokens();
                return true;
            }
            return false;
        }

        inline const bool ScanRight()
        {
            ERRORCLASSFUNCTION(StringTokeniser,ScanRight,!String.IsValid(),RETURN_BOOL)
            char * C_Ptr_2 = String.GetArray();
            SIZE_TYPE TempSize = (C_Ptr-String.GetArray());
            bool ReachedNull = false;

            while(TempSize != String.GetSize())
            {
                if(!ReachedNull)
                {
                    if(C_Ptr_2[TempSize] == END_VALUE)
                    {
                        ReachedNull = true;
                    }
                }
                else
                {
                    if(C_Ptr_2[TempSize] != END_VALUE)
                    {
                        ++TokenNumber;
                        C_Ptr = &C_Ptr_2[TempSize];
                        return true;
                    }
                }
                ++TempSize;
            }
            return false;
        }

        inline const bool ScanLeft()
        {
            ERRORCLASSFUNCTION(StringTokeniser,ScanRight,!String.IsValid(),RETURN_BOOL)
            char * C_Ptr_2 = String.GetArray();
            SIZE_TYPE TempSize = (C_Ptr-String.GetArray());
            bool ReachedNull = false, PassedNull = false;
            while(TempSize > 0)
            {
                if(!ReachedNull)
                {
                    if(C_Ptr_2[TempSize] == END_VALUE)
                    {
                        ReachedNull = true;
                    }
                }
                else
                {
                    if(!PassedNull)
                    {
                        if(C_Ptr_2[TempSize] != END_VALUE)
                        {
                            PassedNull = true;
                        }
                    }
                    else
                    {
                        if(C_Ptr_2[TempSize] == END_VALUE)
                        {
                            ++TempSize;
                            --TokenNumber;
                            C_Ptr = &C_Ptr_2[TempSize];
                            return true;
                        }
                    }
                }
                --TempSize;
            }
            if(ReachedNull)
            {
                --TokenNumber;
                C_Ptr = &C_Ptr_2[TempSize];
                return true;
            }
            return false;
        }

        inline const bool ScanToToken(const SIZE_TYPE Position)
        {
            ERRORCLASSFUNCTION(StringTokeniser,ScanToToken,Position > NumberOfTokens,RETURN_BOOL)
            ERRORCLASSFUNCTION(StringTokeniser,ScanToToken,Position < 1,RETURN_BOOL)

            const bool (TL::StringTokeniser::*FunctionPointer)();

            if(Position < TokenNumber)
            {
                FunctionPointer = &TL::StringTokeniser::ScanLeft;
            }
            else
            {
                FunctionPointer = &TL::StringTokeniser::ScanRight;
            }

            while(TokenNumber != Position)
            {
                ERRORCLASSFUNCTION(StringTokeniser,ScanToToken,!(this->*FunctionPointer)(),RETURN_BOOL)
            }
            return true;
        }

        inline const bool InsertNull()
        {
            ERRORCLASSFUNCTION(StringTokeniser,InsertNull,!IsValid(),RETURN_BOOL)
            *C_Ptr = '\0';
            TokenNumber = 0;
            ScanToFirstToken();
            CountNumberOfTokens();
            return true;
        }

        inline const bool InsertNull(const SIZE_TYPE S)
        {
            ERRORCLASSFUNCTION(StringTokeniser,InsertNull,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(StringTokeniser,InsertNull,(S < 0) || (S > String.GetSize()), RETURN_BOOL)
            C_Ptr = String.GetArray()+S;
            return InsertNull();
        }

        //Behaves strangely - it splits the literal chars rather than interpreting them as things not to read
        inline const bool TokeniseString(const bool SplitOnLiteral = true)
        {
            ERRORCLASSFUNCTION(StringTokeniser,TokeniseString,!String.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(StringTokeniser,TokeniseString,!SplitChars.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(StringTokeniser,TokeniseString,!LiteralChars.IsValid(),RETURN_BOOL)

            TokenNumber = 0;

            C_Ptr = String.GetArray();
            SIZE_TYPE TempSize = 0;
            char TempChar = '\0';
            bool LiteralLock = false;
            while(TempSize != String.GetSize())
            {
                //If not locked
                if(!LiteralLock)
                {
                    //If the char we're on is a literal char
                    if(IsCompareChar(C_Ptr[TempSize],LiteralChars.GetArray()))
                    {
                        //Turn on literal string lock
                        LiteralLock = true;
                        
                        //Assign the temp char to our literal string
                        TempChar = C_Ptr[TempSize];
                        if(SplitOnLiteral){C_Ptr[TempSize] = END_VALUE;}
                    }
                    else
                    {
                        if(IsCompareChar(C_Ptr[TempSize],SplitChars.GetArray()))
                        {
                            C_Ptr[TempSize] = END_VALUE;
                        }
                    }
                }
                else
                {
                    if(C_Ptr[TempSize] == TempChar)
                    {
                        LiteralLock = false;
                        //Old style of string tokenisation. Uncomment the below to restore it or ideally
                        //add an optional boolean called 'SplitOnLiteral' set to true
                        if(SplitOnLiteral){C_Ptr[TempSize] = END_VALUE;}
                    }
                }
                ++TempSize;
            }

            ScanToFirstToken();
            CountNumberOfTokens();
            return true;
        }

        const bool SetStringTokeniser(const StringTokeniser &ItemCopy)
        {
            CharArray Temp1, Temp2, Temp3;
            ERRORCLASSFUNCTION(StringTokeniser,SetStringTokeniser,!Temp1.SetArray(ItemCopy.String),RETURN_BOOL)
            ERRORCLASSFUNCTION(StringTokeniser,SetStringTokeniser,!Temp2.SetArray(ItemCopy.SplitChars),RETURN_BOOL)
            ERRORCLASSFUNCTION(StringTokeniser,SetStringTokeniser,!Temp3.SetArray(ItemCopy.LiteralChars),RETURN_BOOL)

            String.Swap(Temp1);
            SplitChars.Swap(Temp2);
            LiteralChars.Swap(Temp3);
            NumberOfTokens = ItemCopy.NumberOfTokens;

            if(ItemCopy.C_Ptr != NULL)
            {
                C_Ptr = &String.GetArray()[(ItemCopy.C_Ptr-ItemCopy.String.GetArray())];
                TokenNumber = ItemCopy.TokenNumber;
            }
            else
            {
                C_Ptr = String.GetArray();
                TokenNumber = 1;
            }
            return true;
        }

        const bool RemoveString()
        {
            ERRORCLASSFUNCTION(StringTokeniser,RemoveString,!IsValid(),RETURN_BOOL)
            //Was originally String.Replace(GetCurrString(),'\0')
            ERRORCLASSFUNCTION(StringTokeniser,RemoveString,!String.Replace(GetCurrString(),'\0',(C_Ptr - String.GetArray())),RETURN_BOOL)
            CountNumberOfTokens();
            ERRORCLASSFUNCTION(StringTokeniser,RemoveString,!ScanToFirstToken(),RETURN_BOOL)
            return true;
        }

        const bool ReplaceString(const CharArray &Item)
        {
            ERRORCLASSFUNCTION(StringTokeniser,ReplaceString,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(StringTokeniser,ReplaceString,!Item.IsValid(),RETURN_BOOL)

            ERRORCLASSFUNCTION(StringTokeniser,ReplaceString,!String.Replace(GetCurrString(),Item),RETURN_BOOL)
            CountNumberOfTokens();
            ERRORCLASSFUNCTION(StringTokeniser,ReplaceString,!ScanToFirstToken(),RETURN_BOOL)
            return true;
        }

        const bool AppendString(const CharArray &ItemCopy)
        {
            ERRORCLASSFUNCTION(StringTokeniser,AppendString,!ItemCopy.IsValid(),RETURN_BOOL)
            CharArray Temp;
            ERRORCLASSFUNCTION(StringTokeniser,AppendString,!Temp.SetArray(String),RETURN_BOOL)
            ERRORCLASSFUNCTION(StringTokeniser,AppendString,!Temp.Append('\0'),RETURN_BOOL)
            ERRORCLASSFUNCTION(StringTokeniser,AppendString,!Temp.Append(ItemCopy),RETURN_BOOL)
            String.Swap(Temp);
            C_Ptr = String.GetArray();
            CountNumberOfTokens();
            ERRORCLASSFUNCTION(StringTokeniser,AppendString,!ScanToFirstToken(),RETURN_BOOL)
            return true;
        }

        void TransferFrom(StringTokeniser &Temp)
        {
            String.TransferFrom(Temp.String);
            SplitChars.TransferFrom(Temp.SplitChars);
            LiteralChars.TransferFrom(Temp.LiteralChars);
            C_Ptr = Temp.C_Ptr;
            TokenNumber = Temp.TokenNumber;
            NumberOfTokens = Temp.NumberOfTokens;

            Temp.C_Ptr = NULL;
            Temp.TokenNumber = 0;
            Temp.NumberOfTokens = 0;
            return;
        }

        StringTokeniser &operator=(const StringTokeniser &ItemCopy)
        {
            ERRORCLASSFUNCTION(StringTokeniser,operator=,!SetStringTokeniser(ItemCopy),RETURN_THIS)
            return *this;
        }
};

}

#undef END_VALUE
#endif
