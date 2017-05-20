#ifndef G_CHARARRAYLOADER_H
#define G_CHARARRAYLOADER_H

/*
Generated on: 2013-11-30-21.56.51

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include "Loader.h"
#include "CharArray.h"

namespace TL
{

class CharArrayLoader : public Loader<CharArray>
{
	private:
	protected:

	public:
		CharArrayLoader(){Open();}
		~CharArrayLoader(){Close();}

        const bool LoadCharArray(const CharArray &Data)
        {
            ERRORCLASSFUNCTION(CharArrayLoader,LoadCharArray,!Data.IsValid(),RETURN_BOOL)

            ERRORCLASSFUNCTION(CharArrayLoader,LoadCharArray,!AddNodeEndNext(),RETURN_BOOL)
            ERRORCLASSFUNCTION(CharArrayLoader,LoadCharArray,!GetEndItem().SetArray(Data),RemoveNodeEnd();RETURN_BOOL)
            return true;
        }
};

}


#endif
