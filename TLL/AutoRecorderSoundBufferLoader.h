#ifndef G_AUTORECORDERSOUNDBUFFERLOADER_H
#define G_AUTORECORDERSOUNDBUFFERLOADER_H

/*
Generated on: 2012-08-28-21.43.46

Version: 2.1
*/

/*
Notes:
SFML 2.0 friendly version
Previously called AutoBufferRecorderLoader
*/

/*
Suggestions for improvements:


*/

#include "SoundBufferRecorderOffset.h"
#include "SoundBufferLoader.h"

namespace TL
{

class AutoRecorderSoundBufferLoader : public SoundBufferLoader, protected SoundBufferRecorderOffset
{
	private:
	protected:
        using sf::SoundBufferRecorder::start;
        using sf::SoundBufferRecorder::stop;

        float RecordOffset;

	public:

		AutoRecorderSoundBufferLoader(){Open();}
		~AutoRecorderSoundBufferLoader(){Close();}

		void Open()
		{
		    SoundBufferLoader::Open();
		    RecordOffset = 0.4f;
		}

        void Close()
        {
            SoundBufferLoader::Close();
            RecordOffset = 0.4f;
        }

        void Reset()
        {
            Close(); Open();
        }

        void SetRecordOffset(float Data)
        {
            RecordOffset = Data;
        }

        virtual const bool StartRecording()
        {
            //Changed from CanCapture to isAvailable
            ERRORCLASSFUNCTION(AutoRecorderSoundBufferLoader,StartRecording,!sf::SoundBufferRecorder::isAvailable(),RETURN_BOOL)
            start();
            return true;
        }

        virtual const bool StopRecording()
        {
            stop();
            ERRORCLASSFUNCTION(AutoRecorderSoundBufferLoader,Stop,!LoadSoundBuffer(GetBufferOffset(RecordOffset)),RETURN_BOOL)
            return true;
        }


};

}

#endif
