#ifndef G_SOUNDBUFFERRECORDEROFFSET_H
#define G_SOUNDBUFFERRECORDEROFFSET_H

/*
Generated on: 2012-08-30-17.34.38

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include <SFML/Audio.hpp>

namespace TL
{

class SoundBufferRecorderOffset : public sf::SoundBufferRecorder
{
	private:
	protected:
	public:
		SoundBufferRecorderOffset(){}
		~SoundBufferRecorderOffset(){}

        const sf::SoundBuffer GetBufferOffset(const float Offset)
        {
            sf::SoundBuffer Data;
            int Spacing = GetBuffer().GetSampleRate() * GetBuffer().GetChannelsCount() * Offset;
            Data.LoadFromSamples(GetBuffer().GetSamples()+Spacing,GetBuffer().GetSamplesCount() - Spacing, GetBuffer().GetChannelsCount(), GetBuffer().GetSampleRate());
            return Data;
        }
};

}

#endif
