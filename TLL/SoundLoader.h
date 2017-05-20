#ifndef G_SOUNDLOADER_H
#define G_SOUNDLOADER_H

/*
Generated on: 2012-08-23-18.14.37

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include <SFML/Audio.hpp>
#include "Loader.h"

namespace TL
{

class SoundLoader : public Loader<sf::Sound>
{
	private:
	protected:
	public:
		SoundLoader(){Open();}
		~SoundLoader(){Close();}


        const bool LoadSound(const sf::SoundBuffer &Data)
        {
            ERRORCLASSFUNCTION(SoundLoader,LoadSound,!AddNodeEndNext(),RETURN_BOOL)
            GetEndItem().SetBuffer(Data);
            return true;
        }

        const bool LoadFromList(const TemplateList<sf::SoundBuffer> &Sounds)
        {
            ERRORCLASSFUNCTION(SoundLoader,LoadFromList,!Sounds.IsValid(),RETURN_BOOL)
            TemplateIterator<sf::SoundBuffer> Temp;
            Temp = Sounds;
            do
            {
                ERRORCLASSFUNCTION(SoundLoader,LoadFromList,!LoadSound(Temp.GetCurrItem()),RETURN_BOOL)
            }while(Temp.CurrNext());
            return true;
        }

        void PlayAll()
        {

            if(!IsValid()){return;}
            TemplateIteratorNonConst<sf::Sound> Temp = *this;

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
            TemplateIteratorNonConst<sf::Sound> Temp = *this;

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
            TemplateIteratorNonConst<sf::Sound> Temp = *this;

            Temp.CurrToStart();
            do
            {
                Temp.GetCurrItem().Pause();
            }while(Temp.CurrNext());
            return;
        }

        void LoopAll(const bool IsEnabled)
        {
            if(!IsValid()){return;}
            TemplateIteratorNonConst<sf::Sound> Temp = *this;

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
