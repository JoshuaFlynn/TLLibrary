#ifndef G_AUTOIMAGELOADER_H
#define G_AUTOIMAGELOADER_H

/*
Generated on: 2012-08-23-11.32.18

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include "ImageLoader.h"
#include "StringTokeniser.h"
#include "FileProcStringTokeniser.h"

namespace TL
{

class AutoImageLoader : public ImageLoader
{
	private:
	protected:
	public:
		AutoImageLoader(){Open();}
		~AutoImageLoader(){Close();}

		const bool LoadFromFile(const CharArray &File, const CharArray &SplitChars = "\n")
		{
		    ERRORCLASSFUNCTION(AutoImageLoader,LoadFromFile,!File.IsValid(),RETURN_BOOL)
		    ERRORCLASSFUNCTION(AutoImageLoader,LoadFromFile,!SplitChars.IsValid(),RETURN_BOOL)
		    FileProcStringTokeniser Temp;
		    ERRORCLASSFUNCTION(AutoImageLoader,LoadFromFile,!Temp.OpenFile(File,"r"),RETURN_BOOL)
		    StringTokeniser Temp3;

		    ERRORCLASSFUNCTION(AutoImageLoader,LoadFromFile,!Temp3.CopyStringTokeniser(Temp.ToStringTokeniser(SplitChars)),RETURN_BOOL)
		    ERRORCLASSFUNCTION(AutoImageLoader,LoadFromFile,Temp3.GetNumberOfTokens() < 1,RETURN_BOOL)

		    do
		    {
                ERRORCLASSFUNCTION(AutoImageLoader,LoadFromFile,!LoadImage(Temp3.GetCurrString()),)
		    }while(Temp3.ScanRight());
		    return true;
		}

};


}

#endif
