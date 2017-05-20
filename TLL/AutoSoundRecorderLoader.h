#ifndef G_AUTOSOUNDRECORDERLOADER_H
#define G_AUTOSOUNDRECORDERLOADER_H

/*
Generated on: 2012-08-29-02.16.12

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/
#include "AutoBufferRecorderLoader.h"
#include "SoundLoader.h"

namespace TL
{

class AutoSoundRecorderLoader
{
	private:
	protected:
        AutoBufferRecorderLoader RecorderLoaderData;
        SoundLoader SoundLoaderData;

	public:

		void Open(){RecorderLoaderData.Open();SoundLoaderData.Open();}
		void Close(){RecorderLoaderData.Close(); SoundLoaderData.Close();}
        void Reset()
        {
            Close(); Open();
        }

		AutoSoundRecorderLoader(){ Open(); }
        ~AutoSoundRecorderLoader(){ Close(); }

        const bool StartRecording()
        {
            ERRORCLASSFUNCTION(AutoBufferRecorderLoader,StartRecording,!RecorderLoaderData.StartRecording(),RETURN_BOOL)
            return true;
        }

        const bool StopRecording()
        {
            ERRORCLASSFUNCTION(AutoBufferRecorderLoader,Stop,!RecorderLoaderData.StopRecording(),RETURN_BOOL)
            ERRORCLASSFUNCTION(AutoBufferRecorderLoader,Stop,!SoundLoaderData.LoadSound(RecorderLoaderData.GetEndItem()),RETURN_BOOL)
            return true;
        }

        sf::Sound &GetSound(const SIZE_TYPE N)
        {
            return SoundLoaderData[N];
        }

        sf::SoundBuffer &GetSoundBuffer(const SIZE_TYPE N)
        {
            return RecorderLoaderData[N];
        }

        sf::Sound &GetStartSound()
        {
            return SoundLoaderData.GetStartItem();
        }

        sf::SoundBuffer &GetStartSoundBuffer()
        {
            return RecorderLoaderData.GetStartItem();
        }

        sf::Sound &GetEndSound()
        {
            return SoundLoaderData.GetEndItem();
        }

        sf::SoundBuffer &GetEndSoundBuffer()
        {
            return RecorderLoaderData.GetEndItem();
        }

        void PlayAll(){SoundLoaderData.PlayAll();}
        void StopAll(){SoundLoaderData.StopAll();}
        void PauseAll(){SoundLoaderData.PauseAll();}
        void LoopAll(const bool IsEnabled){SoundLoaderData.LoopAll(IsEnabled);}
        void LoopAllEnabled(){SoundLoaderData.LoopAll(true);}
        void LoopAllDisabled(){SoundLoaderData.LoopAll(false);}

        const bool RemoveEnd()
        {
            if(SoundLoaderData.IsEmpty() && RecorderLoaderData.IsEmpty())
            {
                return true;
            }
            ERRORCLASSFUNCTION(AutoSoundRecorderLoader,RemoveEnd,!SoundLoaderData.RemoveNodeEnd(),RETURN_BOOL)
            ERRORCLASSFUNCTION(AutoSoundRecorderLoader,RemoveEnd,!RecorderLoaderData.RemoveNodeEnd(),RETURN_BOOL)
        }
};

}

#endif
