#ifndef G_SOUNDBUFFERLOADER_H
#define G_SOUNDBUFFERLOADER_H

/*
Generated on: 2012-08-24-10.25.14

Version: 1.1
*/

/*
Notes:
Modification to be compatible with 2.1 with lower case function calls

*/

/*
Suggestions for improvements:


*/

#include "Loader.h"
#include <SFML/Audio.hpp>
#include "FileProc.h"

namespace TL
{

class SoundBufferLoader : public Loader<sf::SoundBuffer>
{
	private:
	protected:
	public:
		SoundBufferLoader(){Open();}
		~SoundBufferLoader(){Close();}

		const bool LoadSoundBuffer(const CharArray &FileName)
        {
            ERRORCLASSFUNCTION(SoundBufferLoader,LoadSoundBuffer,!FileName.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(SoundBufferLoader,LoadSoundBuffer,!FileExists(FileName.GetArray()),RETURN_BOOL)

            ERRORCLASSFUNCTION(SoundBufferLoader,LoadSoundBuffer,!AddNodeEndNext(),RETURN_BOOL)
            ERRORCLASSFUNCTION(SoundBufferLoader,LoadSoundBuffer,!GetEndItem().loadFromFile(FileName.GetArray()),RemoveNodeEnd();RETURN_BOOL)
            return true;
        }

        const bool LoadSoundBuffer(const sf::SoundBuffer &ItemCopy)
        {
            ERRORCLASSFUNCTION(SoundBufferLoader,LoadSoundBuffer,!AddNodeEndNext(ItemCopy),RETURN_BOOL)
            return true;
        }

};

void PlaySound(const sf::SoundBuffer &ItemCopy)
{
    sf::Sound SoundPlay;
    
    SoundPlay.setBuffer(ItemCopy);
    SoundPlay.Play();
    
    while(SoundPlay.getStatus() == sf::Sound::Playing);
    return;
}

}

#endif
