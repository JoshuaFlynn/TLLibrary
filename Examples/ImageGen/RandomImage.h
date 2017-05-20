#ifndef G_RANDOMIMAGE_H
#define G_RANDOMIMAGE_H

/*
Generated on: 2013-10-09-17.14.33

Version: 3.0
*/

/*
Notes:
Massive upgrade to make random image generation more useful

Fixed to work with SFML's new setup
Added Directory.h

*/

/*
Suggestions for improvements:


*/

#include <SFML/Graphics.hpp>
#include "FileProc.h"
#include "TemplateArray.h"
#include "Directory.h"
#include "Maths.h"
#include "Random.h"
#include "RandomImageAlgorithm.h"

namespace TL
{

const int GetFullRGB()
{
    return 255;
}

const int GetEmptyRGB()
{
    return 0;
}

const int GetHalfRGB()
{
    return 128;
}

const int GetRandomRGB()
{
    switch(GetRandomInt()%3)
    {
        case 0:
        return GetFullRGB();
        break;

        case 1:
        return GetEmptyRGB();
        break;

        case 2:
        return GetHalfRGB();
    }
    return 0;
}

class RandomImage
{
	private:
		RandomImage &operator==(const RandomImage &ItemCopy){return *this;}
		RandomImage &operator=(const RandomImage &ItemCopy){return *this;}
		RandomImage &operator+(const RandomImage &ItemCopy){return *this;}
		RandomImage &operator-(const RandomImage &ItemCopy){return *this;}
		RandomImage &operator[](const int ItemCopy){return *this;}

	protected:

        sf::Image DefaultImage;
        TemplateArray< sf::Image > Image;

		void Clear(){DefaultImage = sf::Image();}
	public:

		RandomImage(){Open(); Clear();}
		~RandomImage(){Close(); Clear();}

		void Open(){Image.Open(); }
		void Close(){Image.Close();}
		void Reset(){Close(); Open();}

		const bool IsEmpty() const { return Image.IsEmpty(); }
		const bool IsValid() const { return Image.IsValid(); }

		void TransferFrom(RandomImage &ItemCopy){Image.TransferFrom(ItemCopy.Image);}
		void Swap(RandomImage &ItemCopy){Image.Swap(ItemCopy.Image);}
        
        const bool WriteImage(const CharArray &File) const
        {
            ERRORCLASSFUNCTION(RandomImage,WriteImage,!File.IsValid(),return CharArray();)
            ERRORCLASSFUNCTION(RandomImage,WriteImage,!Image.IsValid(),return CharArray();)
            ERRORCLASSFUNCTION(RandomImage,WriteImage,!Image[0].saveToFile(File.GetArray()),return CharArray();)
            
            return true;
        }
        
        const CharArray WriteImageArgV(const CharArray &ArgV) const
        {
            ERRORCLASSFUNCTION(RandomImage,WriteImage,!ArgV.IsValid(),return CharArray();)
            ERRORCLASSFUNCTION(RandomImage,WriteImage,!Image.IsValid(),return CharArray();)

            Directory Dir;

            ERRORCLASSFUNCTION(RandomImage,WriteImage,!Dir.SetDirArgV(ArgV),return CharArray();)

            CharArray Temp2;

            ERRORCLASSFUNCTION(RandomImage,WriteImage,!Temp2.SetArray(GetRandomTemplateArray(20,isalnum)),return CharArray();)
            ERRORCLASSFUNCTION(RandomImage,WriteImage,!Temp2.IsValid(),return CharArray();)
            ERRORCLASSFUNCTION(RandomImage,WriteImage,!Temp2.Append(".png"),return CharArray();)
            ERRORCLASSFUNCTION(RandomImage,WriteImage,!Temp2.SetArray(Dir.AppendFileName(Temp2)),return CharArray();)
            ERRORCLASSFUNCTION(RandomImage,WriteImage,!Temp2.IsValid(),return CharArray();)
            ERRORCLASSFUNCTION(RandomImage,WriteImage,FileExists(Temp2.GetArray()),return CharArray();)

            ERRORCLASSFUNCTION(RandomImage,WriteImage,!Image[0].saveToFile(Temp2.GetArray()),return CharArray();)
            
            return Temp2;
        }

		const bool SetImage(const sf::Image &ItemCopy)
		{
            
		    //if(ItemCopy.getWidth() < 1)
		    if(ItemCopy.getSize().x < 1)
		    {
		        Image.Open();
		        return true;
		    }

		    TemplateArray<sf::Image> Temp2;
		    ERRORCLASSFUNCTION(RandomImage,SetImage,!Temp2.SetSize(1),RETURN_BOOL)
		    //ERRORCLASSFUNCTION(RandomImage,SetImage,!Temp2[0].LoadFromPixels(ItemCopy.GetWidth(),ItemCopy.GetHeight(),ItemCopy.GetPixelsPtr()),RETURN_BOOL)
		    Temp2[0].create(ItemCopy.getSize().x,ItemCopy.getSize().y,ItemCopy.getPixelsPtr());
            Image.TransferFrom(Temp2);
            return true;
		}

		const sf::Image &GetImage() const
		{
		    if(Image.IsValid()){return Image[0];}
		    return DefaultImage;
		}

		const bool CreateRandomImage(unsigned int Width, unsigned int Height, const int (*RandomAlgorithm)(void))
		{
		    ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,Width < 1,RETURN_BOOL)
		    ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,Height < 1,RETURN_BOOL)
		    ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,RandomAlgorithm == NULL,RETURN_BOOL)
            TemplateArray<sf::Image> ItemCopy;
            TemplateArray<sf::Uint8> Temp;

            ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,!ItemCopy.SetSize(1),RETURN_BOOL)
            ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,!Temp.SetSize((Width * Height) * 4),RETURN_BOOL)

            SIZE_TYPE Iter = 0;

            while(Iter < Temp.GetSize() )
            {
                Temp[Iter] = Modulus(RandomAlgorithm(),256);
                Temp[Iter+1] = Modulus(RandomAlgorithm(),256);
                Temp[Iter+2] = Modulus(RandomAlgorithm(),256);
                Temp[Iter+3] = 255;

                Iter = Iter+4;
            }

            //ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,!ItemCopy[0].LoadFromPixels(Width,Height,Temp.GetArray()),RETURN_BOOL)
            ItemCopy[0].create(Width,Height,Temp.GetArray());
            Image.Swap(ItemCopy);
            return true;
		}
		
		const bool CreateRandomImageGreyscale(unsigned int Width, unsigned int Height, const int (*RandomAlgorithm)(void))
		{
		    ERRORCLASSFUNCTION(RandomImage,CreateRandomImageGreyscale,Width < 1,RETURN_BOOL)
		    ERRORCLASSFUNCTION(RandomImage,CreateRandomImageGreyscale,Height < 1,RETURN_BOOL)
		    ERRORCLASSFUNCTION(RandomImage,CreateRandomImageGreyscale,RandomAlgorithm == NULL,RETURN_BOOL)
            TemplateArray<sf::Image> ItemCopy;
            TemplateArray<sf::Uint8> Temp;

            ERRORCLASSFUNCTION(RandomImage,CreateRandomImageGreyscale,!ItemCopy.SetSize(1),RETURN_BOOL)
            ERRORCLASSFUNCTION(RandomImage,CreateRandomImageGreyscale,!Temp.SetSize((Width * Height) * 4),RETURN_BOOL)

            SIZE_TYPE Iter = 0;
            
            sf::Uint8 Temp2;

            while(Iter < Temp.GetSize() )
            {
                Temp2 = Modulus(RandomAlgorithm(),256);
                Temp[Iter] = Temp2;
                Temp[Iter+1] = Temp2;
                Temp[Iter+2] = Temp2;
                Temp[Iter+3] = 255;

                Iter = Iter+4;
            }

            //ERRORCLASSFUNCTION(RandomImage,CreateRandomImageGreyscale,!ItemCopy[0].LoadFromPixels(Width,Height,Temp.GetArray()),RETURN_BOOL)
            ItemCopy[0].create(Width,Height,Temp.GetArray());
            Image.Swap(ItemCopy);
            return true;
		}
		
		

		const bool CreateRandomImage(unsigned int Width, unsigned int Height, const int (*RandomAlgorithmR)(void), const int (*RandomAlgorithmG)(void), const int (*RandomAlgorithmB)(void))
		{
		    ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,Width < 1,RETURN_BOOL)
		    ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,Height < 1,RETURN_BOOL)
		    ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,RandomAlgorithmR == NULL,RETURN_BOOL)
		    ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,RandomAlgorithmG == NULL,RETURN_BOOL)
		    ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,RandomAlgorithmB == NULL,RETURN_BOOL)
            TemplateArray<sf::Image> ItemCopy;
            TemplateArray<sf::Uint8> Temp;

            ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,!ItemCopy.SetSize(1),RETURN_BOOL)
            ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,!Temp.SetSize((Width * Height) * 4),RETURN_BOOL)

            SIZE_TYPE Iter = 0;

            while(Iter < Temp.GetSize() )
            {
                Temp[Iter] = Modulus(RandomAlgorithmR(),256);
                Temp[Iter+1] = Modulus(RandomAlgorithmG(),256);
                Temp[Iter+2] = Modulus(RandomAlgorithmB(),256);
                Temp[Iter+3] = 255;

                Iter = Iter+4;
            }

            //ERRORCLASSFUNCTION(RandomImage,CreateRandomImage,!ItemCopy[0].LoadFromPixels(Width,Height,Temp.GetArray()),RETURN_BOOL)
            ItemCopy[0].create(Width,Height,Temp.GetArray());
            Image.Swap(ItemCopy);
            return true;
		}
		
		const bool CreateRandomImageHelper(const int Width, const int Height,uint8_t R, uint8_t G, uint8_t B, const int (*RandomAlgorithm)(void))
        {
            ERRORCLASSFUNCTION(RandomImage,CreateRandomImageHelper,Width < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(RandomImage,CreateRandomImageHelper,Height < 1,RETURN_BOOL)
            
            ERRORCLASSFUNCTION(RandomImage,CreateRandomImageHelper,R > 3,RETURN_BOOL)
            ERRORCLASSFUNCTION(RandomImage,CreateRandomImageHelper,G > 3,RETURN_BOOL)
            ERRORCLASSFUNCTION(RandomImage,CreateRandomImageHelper,B > 3,RETURN_BOOL)
            
            if(R == 3 || G == 3 || B == 3)
            {
                ERRORCLASSFUNCTION(RandomImage,CreateRandomImageHelper,RandomAlgorithm == NULL,RETURN_BOOL)
            }
        
            const int (*FunctionPointerList[4])(void) = {GetEmptyRGB,GetHalfRGB,GetFullRGB,RandomAlgorithm};
            return CreateRandomImage(Width,Height,FunctionPointerList[R],FunctionPointerList[G],FunctionPointerList[B]);
        }
		
		const bool CreateRandomImageType(unsigned int Width, unsigned int Height, const int (*RandomAlgorithm)(void), const CharArray &Type)
		{
            ERRORCLASSFUNCTION(RandomImage,CreateRandomImageType,Width < 1,RETURN_BOOL)
		    ERRORCLASSFUNCTION(RandomImage,CreateRandomImageType,Height < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(RandomImage,CreateRandomImageType,RandomAlgorithm == NULL,RETURN_BOOL)
            ERRORCLASSFUNCTION(RandomImage,CreateRandomImageHelperType,!Type.IsValid(),RETURN_BOOL)
            
            if(Type[0] == 'b' || Type[0] == 'B')
            {
            
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Blue/Red/White",strlen("Blue/Red/White"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,3,3,2,RandomAlgorithm);
                }
                
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Blue/Black",strlen("Blue/Black"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,0,0,3,RandomAlgorithm);
                }
                
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Blue/Cyan",strlen("Blue/Cyan"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,0,3,2,RandomAlgorithm);
                }
                
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Blue/Purple",strlen("Blue/Purple"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,3,0,2,RandomAlgorithm);
                }
                
                if(Compare(Type.GetArray(),Type.GetSize()-1,"Blue/Pink",strlen("Blue/Pink"),true))
                {
                    return CreateRandomImageHelper(Width,Height,3,1,2,RandomAlgorithm);
                }
                
                if(Compare(Type.GetArray(),Type.GetSize()-1,"Black",strlen("Black"),true))
                {
                    return CreateRandomImageHelper(Width,Height,0,0,0,RandomAlgorithm);
                }
            
            }
            
            else if(Type[0] == 'g' || Type[0] == 'G')
            {
            
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Grey",strlen("Grey"),true) )
                {
                    return CreateRandomImageGreyscale(Width,Height,RandomAlgorithm);
                }
            
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Green/Yellow/White",strlen("Green/Yellow/White"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,3,2,3,RandomAlgorithm);
                }
                
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Green/Black",strlen("Green/Black"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,0,3,0,RandomAlgorithm);
                }
                
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Green/Blue/Cyan/Black",strlen("Green/Blue/Cyan/Black"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,0,3,3,RandomAlgorithm);
                }
                
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Green/Cyan",strlen("Green/Cyan"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,0,2,3,RandomAlgorithm);
                }
                
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Green/Yellow",strlen("Green/Yellow"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,3,2,0,RandomAlgorithm);
                }
                
                if(Compare(Type.GetArray(),Type.GetSize()-1,"Green/Blue",strlen("Green/Blue"),true))
                {
                    return CreateRandomImageHelper(Width,Height,0,3,1,RandomAlgorithm);
                }
                
                if(Compare(Type.GetArray(),Type.GetSize()-1,"Green/Orange",strlen("Green/Orange"),true))
                {
                    return CreateRandomImageHelper(Width,Height,3,1,0,RandomAlgorithm);
                }
            
            }
            
            else if(Type[0] == 'y' || Type[0] == 'Y')
            {
		
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Yellow/White",strlen("Yellow/White"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,2,2,3,RandomAlgorithm);
                }
                
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Yellow/Red/Orange",strlen("Yellow/Red/Orange"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,2,3,0,RandomAlgorithm);
                }
                
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Yellow/Orange/Green/Black",strlen("Yellow/Orange/Green/Black"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,3,3,0,RandomAlgorithm);
                }
                
            }
            
            else if(Type[0] == 'r' || Type[0] == 'R')
            {
            
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Red/Blue/White",strlen("Red/Blue/White"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,3,3,2,RandomAlgorithm);
                }
            
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Red/Yellow/White",strlen("Red/Yellow/White"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,2,3,3,RandomAlgorithm);
                }

                if( Compare(Type.GetArray(),Type.GetSize()-1,"Red/Blue/Purple/Black",strlen("Red/Blue/Purple/Black"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,3,0,3,RandomAlgorithm);
                }
                
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Red/Pink/Purple",strlen("Red/Pink/Purple"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,2,0,3,RandomAlgorithm);
                }
                
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Red/Blue/Green",strlen("Red/Blue/Green"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,3,3,3,RandomAlgorithm);
                }
                
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Red/Black",strlen("Red/Black"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,3,0,0,RandomAlgorithm);
                }
                
                if(Compare(Type.GetArray(),Type.GetSize()-1,"Red/Blue/Purple",strlen("Red/Blue/Purple"),true))
                {
                    return CreateRandomImageHelper(Width,Height,1,0,3,RandomAlgorithm);
                }
                
                if(Compare(Type.GetArray(),Type.GetSize()-1,"Rainbow",strlen("Rainbow"),true))
                {
                    return CreateRandomImageHelper(Width,Height,3,3,3,RandomAlgorithm);
                }
            
            }
            
            else if(Type[0] == 'C' || Type[0] == 'c')
            {
            
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Cyan/White",strlen("Cyan/White"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,3,2,2,RandomAlgorithm);
                }
                
                if(Compare(Type.GetArray(),Type.GetSize()-1,"Cyan/Green",strlen("Cyan/Green"),true))
                {
                    return CreateRandomImageHelper(Width,Height,0,1,3,RandomAlgorithm);
                }
                
                if(Compare(Type.GetArray(),Type.GetSize()-1,"Cyan/Blue/Purple",strlen("Cyan/Blue/Purple"),true))
                {
                    return CreateRandomImageHelper(Width,Height,1,3,2,RandomAlgorithm);
                }
            
            }
            
            else if(Type[0] == 'P' || Type[0] == 'p')
            {
            
                if( Compare(Type.GetArray(),Type.GetSize()-1,"Purple/White",strlen("Purple/White"),true) )
                {
                    return CreateRandomImageHelper(Width,Height,2,3,2,RandomAlgorithm);
                }
            
                if(Compare(Type.GetArray(),Type.GetSize()-1,"Pink/Peach",strlen("Pink/Peach"),true))
                {
                    return CreateRandomImageHelper(Width,Height,2,1,3,RandomAlgorithm);
                }
                
                //Rubard/Custard
                if(Compare(Type.GetArray(),Type.GetSize()-1,"Pink/Yellow",strlen("Pink/Yellow"),true))
                {
                    return CreateRandomImageHelper(Width,Height,2,3,1,RandomAlgorithm);
                }
                
                if(Compare(Type.GetArray(),Type.GetSize()-1,"Purple/Blue",strlen("Purple/Blue"),true))
                {
                    return CreateRandomImageHelper(Width,Height,3,0,1,RandomAlgorithm);
                }
            
            }
            
            else if (Type[0] == 'l' || Type[0] == 'L')
            {
            
                if(Compare(Type.GetArray(),Type.GetSize()-1,"LightGreen/Yellow/Cyan",strlen("LightGreen/Yellow/Cyan"),true))
                {
                    return CreateRandomImageHelper(Width,Height,1,2,3,RandomAlgorithm);
                }
                
                if(Compare(Type.GetArray(),Type.GetSize()-1,"LightGreen/Brown",strlen("LightGreen/Brown"),true))
                {
                    return CreateRandomImageHelper(Width,Height,1,3,0,RandomAlgorithm);
                }
                
                if(Compare(Type.GetArray(),Type.GetSize()-1,"LightGreen/Yellow",strlen("LightGreen/Yellow"),true))
                {
                    return CreateRandomImageHelper(Width,Height,3,2,1,RandomAlgorithm);
                }
            
            }
            else if (Type[0] == 'w' || Type[0] == 'W')
            {
            
                if(Compare(Type.GetArray(),Type.GetSize()-1,"White",strlen("White"),true))
                {
                    return CreateRandomImageHelper(Width,Height,2,2,2,RandomAlgorithm);
                }
                
            }
		
            ERRORCLASSFUNCTION(RandomImage,CreateRandomImageType,true,RETURN_BOOL)
		
            return false;
		}

};

}

#endif
