#ifndef G_AUTOSOUNDBUFFERLOADER_H
#define G_AUTOSOUNDBUFFERLOADER_H

/*
Generated on: 2012-08-24-16.27.09

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include "FileProcStringTokeniser.h"
#include "SoundBufferLoader.h"

namespace TL
{

class AutoSoundBufferLoader : public SoundBufferLoader
{
	private:
	protected:
	public:
		AutoSoundBufferLoader(){Open();}
		~AutoSoundBufferLoader(){Close();}

        const bool LoadFromFile(const CharArray &File, const CharArray &SplitChars = "\n")
		{
		    ERRORCLASSFUNCTION(AutoSoundBufferLoader,LoadFromFile,!File.IsValid(),RETURN_BOOL)
		    ERRORCLASSFUNCTION(AutoSoundBufferLoader,LoadFromFile,!SplitChars.IsValid(),RETURN_BOOL)
		    FileProcStringTokeniser Temp;
		    ERRORCLASSFUNCTION(AutoSoundBufferLoader,LoadFromFile,!Temp.OpenFile(File,"r"),RETURN_BOOL)
		    StringTokeniser Temp3;

		    ERRORCLASSFUNCTION(AutoSoundBufferLoader,LoadFromFile,!Temp3.CopyStringTokeniser(Temp.ToStringTokeniser(SplitChars)),RETURN_BOOL)
		    ERRORCLASSFUNCTION(AutoSoundBufferLoader,LoadFromFile,Temp3.GetNumberOfTokens() < 1,RETURN_BOOL)

		    do
		    {
                ERRORCLASSFUNCTION(AutoSoundBufferLoader,LoadFromFile,!LoadSoundBuffer(Temp3.GetCurrString()),)
		    }while(Temp3.ScanRight());
		    return true;
		}
};

}

#endif
