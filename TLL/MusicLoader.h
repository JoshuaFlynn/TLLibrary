#ifndef G_MUSICLOADER_H
#define G_MUSICLOADER_H

/*
Generated on: 2013-11-30-18.35.37

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include <SFML/Audio.hpp>
#include "FileProc.h"
#include "Loader.h"

namespace TL
{

class MusicLoader : public Loader<sf::Music>
{
	private:
	protected:
	public:
		MusicLoader(){Open();}
		~MusicLoader(){Close();}


        const bool LoadMusic(const CharArray &FileName)
        {
            ERRORCLASSFUNCTION(MusicLoader,LoadMusic,!FileName.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(MusicLoader,LoadMusic,!FileExists(FileName.GetArray()),RETURN_BOOL)

            ERRORCLASSFUNCTION(MusicLoader,LoadMusic,!AddNodeEndNext(),RETURN_BOOL)
            ERRORCLASSFUNCTION(MusicLoader,LoadMusic,!GetEndItem().OpenFromFile(FileName.GetArray()),RemoveNodeEnd();RETURN_BOOL)
            return true;
        }

        void PlayAll()
        {

            if(!IsValid()){return;}
            TemplateIteratorNonConst<sf::Music> Temp = *this;

            Temp.CurrToStart();
            do
            {
                Temp.GetCurrItem().Play();
            }while(Temp.CurrNext());
            return;
        }

        void StopAll()
        {

            if(!IsValid()){return;}
            TemplateIteratorNonConst<sf::Music> Temp = *this;

            Temp.CurrToStart();
            do
            {
                Temp.GetCurrItem().Stop();
            }while(Temp.CurrNext());
            return;
        }

        void PauseAll()
        {

            if(!IsValid()){return;}
            TemplateIteratorNonConst<sf::Music> Temp = *this;

            Temp.CurrToStart();
            do
            {
                Temp.GetCurrItem().Pause();
            }while(Temp.CurrNext());
            return;
        }

        void SetVolumeAll(const float Vol)
        {

            if(!IsValid()){return;}
            TemplateIteratorNonConst<sf::Music> Temp = *this;

            Temp.CurrToStart();
            do
            {
                Temp.GetCurrItem().SetVolume(Vol);
            }while(Temp.CurrNext());
            return;
        }

        void LoopAll(const bool IsEnabled)
        {
            if(!IsValid()){return;}
            TemplateIteratorNonConst<sf::Music> Temp = *this;

            Temp.CurrToStart();
            do
            {
                Temp.GetCurrItem().SetLoop(IsEnabled);
            }while(Temp.CurrNext());
            return;
        }

        void LoopAllEnabled()
        {
            LoopAll(true);
            return;
        }

        void LoopAllDisabled()
        {
            LoopAll(false);
            return;
        }

};

}

#endif
