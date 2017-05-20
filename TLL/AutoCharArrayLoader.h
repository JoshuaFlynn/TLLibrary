#ifndef G_AUTOCHARARRAYLOADER_H
#define G_AUTOCHARARRAYLOADER_H

/*
Generated on: 2013-11-30-22.02.37

Version: 1.3
*/

/*
Notes:

Added split on literal support

Improved efficiency of string tokeniser loading
Added literal chars support for loading

Added constructor

*/

/*
Suggestions for improvements:


*/

#include "CharArrayLoader.h"
#include "StringTokeniser.h"
#include "FileProcStringTokeniser.h"

namespace TL
{

class AutoCharArrayLoader : public CharArrayLoader
{
	private:
	protected:
	public:
		AutoCharArrayLoader(){Open();}
		~AutoCharArrayLoader(){Close();}
		
		AutoCharArrayLoader(const CharArray &File, const CharArray &SplitChars = "\n", const CharArray &LiteralChars = "", const bool SplitOnLiteral = true)
		{
            Open();
            ERRORCLASSFUNCTION(AutoCharArrayLoader,AutoCharArrayLoader,!LoadFromFile(File,SplitChars,LiteralChars),)
		}

		const bool LoadFromFile(const CharArray &File, const CharArray &SplitChars = "\n", const CharArray &LiteralChars = "", const bool SplitOnLiteral = true)
		{
		    ERRORCLASSFUNCTION(AutoCharArrayLoader,LoadFromFile,!File.IsValid(),RETURN_BOOL)
		    ERRORCLASSFUNCTION(AutoCharArrayLoader,LoadFromFile,!SplitChars.IsValid(),RETURN_BOOL)
		    FileProcStringTokeniser Temp;
		    ERRORCLASSFUNCTION(AutoCharArrayLoader,LoadFromFile,!Temp.OpenFile(File,"r"),RETURN_BOOL)
		    StringTokeniser Temp3;

            //Original functional code
		    //ERRORCLASSFUNCTION(AutoCharArrayLoader,LoadFromFile,!Temp3.CopyStringTokeniser(Temp.ToStringTokeniser(SplitChars)),RETURN_BOOL)
		    
		    //More efficient code
		    ERRORCLASSFUNCTION(AutoCharArrayLoader,LoadFromFile,!Temp.CopyToStringTokeniser(Temp3,SplitChars,LiteralChars,SplitOnLiteral),RETURN_BOOL)
		    
		    ERRORCLASSFUNCTION(AutoCharArrayLoader,LoadFromFile,Temp3.GetNumberOfTokens() < 1,RETURN_BOOL)

		    do
		    {
                ERRORCLASSFUNCTION(AutoCharArrayLoader,LoadFromFile,!LoadCharArray(Temp3.GetCurrString()),)
		    }while(Temp3.ScanRight());
		    return true;
		}
		
		const bool WriteToFile(const CharArray &File, const char SplitChar = '\n', const CharArray WriteMode = "a", const SIZE_TYPE From = 1)
		{
            ERRORCLASSFUNCTION(AutoCharArrayLoader,WriteToFile,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(AutoCharArrayLoader,WriteToFile,From < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(AutoCharArrayLoader,WriteToFile,!File.IsValid(),RETURN_BOOL)
		    ERRORCLASSFUNCTION(AutoCharArrayLoader,WriteToFile,!WriteMode.IsValid(),RETURN_BOOL)
		    FileProcEquals Temp;
		    
		    ERRORCLASSFUNCTION(AutoCharArrayLoader,WriteToFile,!Temp.OpenFile(File,WriteMode),RETURN_BOOL)

            TemplateIterator<CharArray> Temp2 = *this;
            
            if(From == 1)
            {
                Temp2.CurrToStart();
            }
            else
            {
                ERRORCLASSFUNCTION(AutoCharArrayLoader,WriteToFile,!Temp2.CurrTo(From),RETURN_BOOL)
            }

		    do{
                ERRORCLASSFUNCTION(AutoCharArrayLoader,WriteToFile,!Temp.WriteArray(Temp2.GetCurrItem().GetArray(),Temp2.GetCurrItem().GetSize()-1),RETURN_BOOL)
                ERRORCLASSFUNCTION(AutoCharArrayLoader,WriteToFile,!Temp.WriteChar(SplitChar),RETURN_BOOL)
		    }while(Temp2.CurrNext());
		    
		    return true;
		}

};


}

#endif
