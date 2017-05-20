#ifndef G_RANDOMIMAGELOADER_H
#define G_RANDOMIMAGELOADER_H

/*
Generated on: 2015-11-02-12.09

Version: 1.0
*/

/*
Notes:

*/

/*
Suggestions for improvements:


*/

#include "ImageLoader.h"
#include "RandomImage.h"

namespace TL
{

class RandomImageLoader : public ImageLoader
{
	private:
	protected:

	public:
		RandomImageLoader(){Open();}
		~RandomImageLoader(){Close();}

		const bool LoadRandomImage(unsigned int Width, unsigned int Height, const int (*RandomAlgorithm)(void), const CharArray &Type)
		{
            DEBUGARRAY("Rendering")
            RandomImage Temp;
            ERRORCLASSFUNCTION(RandomImageLoader,LoadRandomImage,!Temp.CreateRandomImageType(Width,Height,RandomAlgorithm,Type),RETURN_BOOL)
            ERRORCLASSFUNCTION(RandomImageLoader,LoadRandomImage,!LoadImage(Temp.GetImage()),RETURN_BOOL)
            return true;
		}

		const bool LoadRandomImage(unsigned int Width, unsigned int Height, const int (*RandomAlgorithmR)(void), const int (*RandomAlgorithmG)(void), const int (*RandomAlgorithmB)(void))
		{
            RandomImage Temp;
            ERRORCLASSFUNCTION(RandomImageLoader,LoadRandomImage,!Temp.CreateRandomImage(Width,Height,RandomAlgorithmR,RandomAlgorithmG,RandomAlgorithmB),RETURN_BOOL)
            ERRORCLASSFUNCTION(RandomImageLoader,LoadRandomImage,!LoadImage(Temp.GetImage()),RETURN_BOOL)
            return true;
		}
};

}

#endif
