#ifndef G_AUTOMUSICLOADER_H
#define G_AUTOMUSICLOADER_H

/*
Generated on: 2013-11-30-18.40.30

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include <SFML/Audio.hpp>
#include "MusicLoader.h"
#include "FileProcStringTokeniser.h"

namespace TL
{

class AutoMusicLoader : public MusicLoader
{
	private:
	protected:
	public:
		AutoMusicLoader(){Open();}
		~AutoMusicLoader(){Close();}

		const bool LoadFromFile(const CharArray &File, const CharArray &SplitChars = "\n")
		{
		    ERRORCLASSFUNCTION(AutoMusicLoader,LoadFromFile,!File.IsValid(),RETURN_BOOL)
		    ERRORCLASSFUNCTION(AutoMusicLoader,LoadFromFile,!SplitChars.IsValid(),RETURN_BOOL)
		    FileProcStringTokeniser Temp;
		    ERRORCLASSFUNCTION(AutoMusicLoader,LoadFromFile,!Temp.OpenFile(File,"r"),RETURN_BOOL)
		    StringTokeniser Temp3;

		    ERRORCLASSFUNCTION(AutoMusicLoader,LoadFromFile,!Temp3.CopyStringTokeniser(Temp.ToStringTokeniser(SplitChars)),RETURN_BOOL)
		    ERRORCLASSFUNCTION(AutoMusicLoader,LoadFromFile,Temp3.GetNumberOfTokens() < 1,RETURN_BOOL)

		    do
		    {
                ERRORCLASSFUNCTION(AutoMusicLoader,LoadFromFile,!LoadMusic(Temp3.GetCurrString()),)
		    }while(Temp3.ScanRight());
		    return true;
		}

};

}

#endif
