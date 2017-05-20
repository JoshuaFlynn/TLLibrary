#ifndef G_BEATGENERATOR_H
#define G_BEATGENERATOR_H

/*
Generated on: 2013-02-16-21.16.46

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include "Note.h"
#include "Random.h"

namespace TL
{

class BeatGenerator
{
	private:
	protected:
        TemplateArray<bool> Track;
        Note BeatSound;
        unsigned short Tempo;

        void Clear(){Tempo = 0;}

	public:
		BeatGenerator(){Open();}
		BeatGenerator(const std::string &FileName,const unsigned int NumberOfBeats,const unsigned short Tempo)
		{
		    Open();
		    SetBeat(FileName,NumberOfBeats,Tempo);
		}
		~BeatGenerator(){Close();}

		void Open(){Track.Open(); BeatSound.Open(); Clear();}
		void Close(){Track.Close(); BeatSound.Close();}
		void Reset(){Close(); Open();}

		const bool IsValid() const {return Track.IsValid() && (Tempo > 0); }
		const bool IsEmpty() const {return Track.IsEmpty() & (Tempo == 0);}

		const TemplateArray<bool> &GetTrack() const {return Track;}
		const unsigned short &GetTempo() const {return Tempo;}
		const Note &GetBeatSound() const {return BeatSound;}

        const bool SetBeat(const std::string &FileName,const unsigned int NumberOfBeats,const unsigned short Tempo)
        {
            ERRORCLASSFUNCTION(BeatGenerator,SetBeat,FileName.empty(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatGenerator,SetBeat,NumberOfBeats < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatGenerator,SetBeat,Tempo < 1,RETURN_BOOL)

            BeatGenerator Temp;
            ERRORCLASSFUNCTION(BeatGenerator,SetBeat,!Temp.Track.SetSize(NumberOfBeats),RETURN_BOOL)
            Temp.Tempo = Tempo;
            ERRORCLASSFUNCTION(BeatGenerator,SetBeat,!Temp.BeatSound.OpenFromFile(FileName),RETURN_BOOL)

            SIZE_TYPE Iter = 0;
            while(Iter < Temp.Track.GetSize())
            {
                Temp.Track[Iter] = false;
                Iter++;
            }

            TransferFrom(Temp);
            return true;
        }

        const bool GenerateRandomBeat()
        {
            ERRORCLASSFUNCTION(BeatGenerator,GenerateRandomBeat,!IsValid(),RETURN_BOOL)
            SIZE_TYPE Iter = 0;
            bool IsBeat;
            while(Iter < Track.GetSize())
            {
                Track[Iter] = GetRandomBool();
                if(Track[Iter]){IsBeat = true;}

                Iter++;
                if(!IsBeat)
                {
                    if(Iter == Track.GetSize())
                    {
                        Iter = 0;
                    }
                }
            }

            return true;
        }

        const bool PlayBeat()
        {
            ERRORCLASSFUNCTION(BeatGenerator,PlayBeat,!IsValid(),RETURN_BOOL)

            SIZE_TYPE Iter = 0;
            while(Iter < Track.GetSize())
            {
                if(Track[Iter])
                {
                    BeatSound.Play();
                }
                Sleep(Tempo);
                Iter++;
            }
            return true;
        }

        void SetVolume(const float &Volume)
        {
            BeatSound.SetVolume(Volume);
        }

        void SetPitchSFML(const float &Pitch)
        {
            BeatSound.SetPitchSFML(Pitch);
        }

        void SetPitchReal(const float &Pitch)
        {
            BeatSound.SetPitchReal(Pitch);
        }

        void TransferFrom(BeatGenerator &ItemCopy)
        {
            Tempo = ItemCopy.Tempo;
            Track.TransferFrom(ItemCopy.Track);
            BeatSound.TransferFrom(ItemCopy.BeatSound);
        }

        BeatGenerator &operator=(const BeatGenerator &ItemCopy)
		{
		    BeatGenerator Temp;
		    Temp.BeatSound = ItemCopy.BeatSound;
		    ERRORCLASSFUNCTION(BeatGenerator,operator=,!Temp.Track.SetArray(ItemCopy.Track),RETURN_THIS);
		    Temp.Tempo = ItemCopy.Tempo;
            TransferFrom(Temp);
			return *this;
		}
};

}

#endif
