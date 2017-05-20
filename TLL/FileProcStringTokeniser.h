#ifndef G_FILEPROCSTRINGTOKENISER_H
#define G_FILEPROCSTRINGTOKENISER_H

/*
Generated on: 2012-08-24-14.54.42

Version: 2.1
*/

/*
Notes:
Updated CopyTo with SplitOnLiteral support

Added support for SplitOnLiteral

Added constructor that accepts the same style arguments as FileProcEquals etc

*/

/*
Suggestions for improvements:


*/

#include "FileProcEquals.h"
#include "StringTokeniser.h"

namespace TL
{

class FileProcStringTokeniser : public FileProcEquals
{
	private:
	protected:
	public:
		FileProcStringTokeniser(){Open();}
		FileProcStringTokeniser(const CharArray &FileN, const CharArray &AccessFlags)
		{
		    ERRORCLASSFUNCTION(FileProcStringTokeniser,FileProcEquals,!OpenFile(FileN, AccessFlags),)
		}
		~FileProcStringTokeniser(){Close();}

        const StringTokeniser ToStringTokeniser(const CharArray &SplitChars = CharArray(""), const CharArray &LiteralChars = CharArray(""), const bool SplitOnLiteral = true)
        {
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!SplitChars.IsValid(),return StringTokeniser();)
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!LiteralChars.IsValid(),return StringTokeniser();)
            CharArray Temp;

            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!Temp.SetArray(ToCharArray()),return StringTokeniser();)
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!Temp.IsValid(),return StringTokeniser();)

            StringTokeniser Temp2;
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!Temp2.SetString(Temp),return StringTokeniser();)
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!Temp2.SetSplitChars(SplitChars),return StringTokeniser();)
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!Temp2.SetLiteralChars(LiteralChars),return StringTokeniser();)
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!Temp2.TokeniseString(SplitOnLiteral),return StringTokeniser();)

            return Temp2;
        }

        const bool CopyToStringTokeniser(StringTokeniser &To, const CharArray &SplitChars = CharArray(""), const CharArray &LiteralChars = CharArray(""), const bool SplitOnLiteral = true)
        {
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!SplitChars.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!LiteralChars.IsValid(),RETURN_BOOL)
            CharArray Temp;

            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!Temp.SetArray(ToCharArray()),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!Temp.IsValid(),RETURN_BOOL)

            StringTokeniser Temp2;
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!Temp2.SetString(Temp),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!Temp2.SetSplitChars(SplitChars),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!Temp2.SetLiteralChars(LiteralChars),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProcStringTokeniser,ToStringTokeniser,!Temp2.TokeniseString(SplitOnLiteral),RETURN_BOOL)
            To.TransferFrom(Temp2);
            return true;
        }

        const bool WriteStringTokeniser(const StringTokeniser &ItemCopy, const char Seperator = '\n')
        {
            ERRORCLASSFUNCTION(FileProcStringTokeniser,WriteStringTokeniser,!ItemCopy.IsValid(),RETURN_BOOL)
            StringTokeniser Temp;
            ERRORCLASSFUNCTION(FileProcStringTokeniser,WriteStringTokeniser,!Temp.CopyStringTokeniser(ItemCopy),RETURN_BOOL)

            ERRORCLASSFUNCTION(FileProcStringTokeniser,WriteStringTokeniser,Temp.GetNumberOfTokens(),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProcStringTokeniser,WriteStringTokeniser,!Temp.ScanToFirstToken(),RETURN_BOOL)
            do
            {
                ERRORCLASSFUNCTION(FileProcStringTokeniser,WriteStringTokeniser,!WriteArray(Temp.GetCurrString(),strlen(Temp.GetCurrString())),RETURN_BOOL)
            }while(Temp.ScanRight());
            return true;
        }
};

}

#endif
