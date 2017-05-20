#ifndef G_IMAGELOADER_H
#define G_IMAGELOADER_H

/*
Generated on: 2012-08-17-16.38.42

Version: 2.0
*/

/*
Notes:
Added additional image suppot for operations

Fixed for the newer SFML

*/

/*
Suggestions for improvements:


*/

#include <SFML/Graphics.hpp>
#include "Loader.h"
#include "CharArray.h"
#include "FileProc.h"

namespace TL
{

class ImageLoader : public Loader<sf::Image>
{
	private:
	protected:

	public:
		ImageLoader(){Open();}
		~ImageLoader(){Close();}

        const bool LoadImage(const CharArray &FileName)
        {
            ERRORCLASSFUNCTION(ImageLoader,LoadImage,!FileName.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(ImageLoader,LoadImage,!FileExists(FileName.GetArray()),RETURN_BOOL)

            ERRORCLASSFUNCTION(ImageLoader,LoadImage,!AddNodeEndNext(),RETURN_BOOL)
            ERRORCLASSFUNCTION(ImageLoader,LoadImage,!GetEndItem().loadFromFile(FileName.GetArray()),RemoveNodeEnd();RETURN_BOOL)
            return true;
        }
        
        const bool LoadImage(const sf::Image &Image)
        {

            ERRORCLASSFUNCTION(ImageLoader,LoadImage,!AddNodeEndNext(),RETURN_BOOL)
            GetEndItem() = Image;
            return true;
        }
        
        const bool SaveCurrImageToFile(const CharArray &File)
        {
            ERRORCLASSFUNCTION(ImageLoader,SaveCurrImageToFile,!File.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(ImageLoader,SaveCurrImageToFile,!IsValid(),RETURN_BOOL)
            
            ERRORCLASSFUNCTION(ImageLoader,SaveCurrImageToFile,!GetCurrItem().saveToFile(File.GetArray()),RETURN_BOOL)
            return true;
        }
};

}

#endif
