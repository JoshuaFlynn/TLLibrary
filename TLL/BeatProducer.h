#ifndef G_BEATPRODUCER_H
#define G_BEATPRODUCER_H

/*
Generated on: 2015-05-07

Version: 1.1
*/

/*
Notes:
Corrected comparison error

*/

/*
Suggestions for improvements:


*/

#include "TemplateArray.h"
#include "TemplateList.h"
#include "SoundBufferLoader.h"
#include "Maths.h"
#include "Random.h"
#include "SoundLoader.h"
#include "StringTokeniser.h"
#include "FileProcStringTokeniser.h"
#include <unistd.h> //usleep

namespace TL
{

class BeatProducer
{
    private:
    protected:
        TemplateArray<TemplateArray<unsigned int> > Music;
        
        void ClearArray(TemplateArray<unsigned int> &Clear)
        {
            SIZE_TYPE Iter = 0;
            while(Iter < Clear.GetSize())
            {
                Clear[Iter] = 0;
                Iter++;
            }
        }
        
    public:
    
        void TransferFrom(BeatProducer &ItemCopy)
        {
            Music.TransferFrom(ItemCopy.Music);
        }
        void Swap(BeatProducer &ItemCopy)
        {
            Music.Swap(ItemCopy.Music);
        }
    
        BeatProducer(){Open();}
        ~BeatProducer(){Close();}
    
        void Open(){Music.Open();}
        void Close(){Music.Close();}
        void Reset(){Music.Close(); Music.Open();}
    
        const bool IsValid() const {return Music.IsValid();}
        const bool IsEmpty() const {return Music.IsEmpty();}
        
        const TemplateArray<TemplateArray<unsigned int> > &GetMusic() const {return Music;}
        TemplateArray<TemplateArray<unsigned int> > &GetMusic() {return Music;}
        
        inline TemplateArray<unsigned int> &operator[](const SIZE_TYPE ItemCopy)
		{
            return Music[ItemCopy];
		}

		inline const TemplateArray<unsigned int> &operator[](const SIZE_TYPE ItemCopy) const
		{
		    return Music[ItemCopy];
		}
        
        const bool SetBeatProducer(const BeatProducer &ItemCopy)
        {
            ERRORCLASSFUNCTION(BeatProducer,SetBeatProducer,!Music.SetArray(ItemCopy.Music),RETURN_BOOL)
            return true;
        }
        
        const bool SetBeatProducer(const TemplateArray<TemplateArray<unsigned int> > &ItemCopy)
        {
            ERRORCLASSFUNCTION(BeatProducer,SetBeatProducer,!Music.SetArray(ItemCopy),RETURN_BOOL)
            return true;
        }
        
        const bool AppendTrack(const TemplateArray<unsigned int> &ItemCopy)
        {
            ERRORCLASSFUNCTION(BeatProducer,AppendTrack,!Music.Append(ItemCopy),RETURN_BOOL)
            return true;
        }
        
        const bool SetSize(const SIZE_TYPE NumberOfTracks, const SIZE_TYPE TrackLength)
        {
            TemplateArray<TemplateArray<unsigned int> > Temp;
            
            ERRORCLASSFUNCTION(BeatProducer,SetSize,!Temp.SetSize(NumberOfTracks),RETURN_BOOL)
            SIZE_TYPE Iter = 0;
            
            while(Iter < Temp.GetSize())
            {
                ERRORCLASSFUNCTION(BeatProducer,SetSize,!Temp[Iter].SetSize(TrackLength),RETURN_BOOL)
                ClearArray(Temp[Iter]);
                Iter++;
            }
            
            Music.TransferFrom(Temp);
            return true;
        }
        
        const bool ProduceBeat(const SoundBufferLoader &LoadSounds, const CharArray &Composition, const int BeatsPerPart, const int NumberOfInstruments)
        {
            //Check for errors in given infomation
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!LoadSounds.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!Composition.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,NumberOfInstruments < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,static_cast<SIZE_TYPE>(NumberOfInstruments) > LoadSounds.GetSize(),RETURN_BOOL)
            
            //Declare variables
            TemplateArray<TemplateArray<unsigned int> > Temp;
            TemplateArray<unsigned int> Instruments;
            TemplateArray<TemplateArray<unsigned int> > CompositionalPiece;
            TemplateArray<unsigned int> CompositionalTime;
            
            StringTokeniser StringToken;
            
            //Figure out how many parts the person wants
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!StringToken.SetString(Composition),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!StringToken.SetSplitChars(":"),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!StringToken.TokeniseString(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,StringToken.GetNumberOfTokens() < 1,RETURN_BOOL)

            //Set the size of the compositional piece - which will generate one template per part
            //Set the size of compositional time - which determines how many segments each part gets
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!CompositionalPiece.SetSize(StringToken.GetNumberOfTokens(),false),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!CompositionalTime.SetSize(StringToken.GetNumberOfTokens(),false),RETURN_BOOL)
            
            //Split to check that the numbers are a range
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!StringToken.SetSplitChars("-"),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!StringToken.TokeniseString(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,IsOdd(StringToken.GetNumberOfTokens()),RETURN_BOOL)
            
            
            
            SIZE_TYPE Total = 0;
            SIZE_TYPE Iter = 0;
            
            //Brackets to ensure X and Y do not last longer than necessary
            {
                ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!StringToken.ScanToFirstToken(),RETURN_BOOL)
                int X = 0, Y = 0;
                //Extract the information from the supplied text string on the segments, then randomly pick a number for variation
                while(Iter < CompositionalTime.GetSize())
                {
                    X = StringToLong(StringToken.GetCurrString());
                    ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,X < 1,RETURN_BOOL)
                    ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!StringToken.ScanRight(),RETURN_BOOL)
                    Y = StringToLong(StringToken.GetCurrString());
                    ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,Y < 1, RETURN_BOOL)
                    
                    CompositionalTime[Iter] = GetRandomInt(X,Y);
                    
                    //Count how many segments there are total
                    Total = Total + CompositionalTime[Iter];
                    if(!StringToken.ScanRight())
                    {
                        break;
                    }
                    Iter++;
                }
            }
            
            //Set our temporary variable to the number of segments
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!Temp.SetSize(Total),RETURN_BOOL)
            
            //Randomlist stores our list of randomly picked instruments.
            TemplateArray<unsigned int> RandomList;
            
            ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!RandomList.SetSize(NumberOfInstruments+1,false),RETURN_BOOL)
            RandomList[0] = 0; //No sound
            
            //Generate the random list of instruments
            Iter = 1;
            SIZE_TYPE Iter1 = 0;
            unsigned int RandomIter = 0;
            
            while(Iter < RandomList.GetSize())
            {
                Iter1 = 0;
                while(IsItem(RandomIter = static_cast<unsigned int>( GetRandomInt(1,LoadSounds.GetSize()) ),RandomList.GetArray(),RandomList.GetSize()) )
                {
                    //If we're looping for too long searching for a random answer, we step in with a brute force approach
                    if(Iter1 > 10)
                    {
                        if(!IsItem(RandomIter = (Iter1-10),RandomList.GetArray(),RandomList.GetSize()))
                        {
                            RandomIter = (Iter1-10);
                            break;
                        }
                    }
                    Iter1++;
                }
                
                
                if(!IsItem(RandomIter,RandomList.GetArray(),RandomList.GetSize()))
                {
                    RandomList[Iter] = RandomIter;
                    Iter++;
                }
            }
            
            //Generate the compositional piece which will then be used to repeat on intro etc
            
            Iter = 0;
            Iter1 = 0;
            while(Iter < CompositionalPiece.GetSize())
            {
                ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,!CompositionalPiece[Iter].SetSize(BeatsPerPart,false),RETURN_BOOL)
                
                Iter1 = 0;
                int RandomIter = 0;
                while(Iter1 < static_cast<SIZE_TYPE>(BeatsPerPart))
                {
                    RandomIter = GetRandomInt(0,RandomList.GetSize()-1);
                    CompositionalPiece[Iter][Iter1] = RandomList[RandomIter];
                    ERRORCLASSFUNCTION(BeatProducer,ProduceBeat,static_cast<SIZE_TYPE>(CompositionalPiece[Iter][Iter1]) > LoadSounds.GetSize(),RETURN_BOOL)
                    Iter1++;
                }
                Iter++;
            }
            
            Iter = 0;
            Iter1 = 0;
            SIZE_TYPE Iter2 = 0;

            //Now we have everything to generate the song
            //We count through the compositional time because we need to know how long each segment lasts for
            while(Iter < CompositionalTime.GetSize())
            {
                Iter1 = 0;
                //We then count through the number of segments per section
                while(Iter1 < static_cast<SIZE_TYPE>(CompositionalTime[Iter]))
                {
                    //We copy the template compositional piece of this section
                    Temp[Iter2].SetArray(CompositionalPiece[Iter]); //This variable (iter) keeps track of the time
                    Iter1++; //This variable keeps track of the length of this particular segment
                    Iter2++; //This variable keeps track of the total amount
                }
                Iter++;
            }
            //Our music should be generated! Yay... now we leave with it.
            Music.TransferFrom(Temp);
            return true;
        }
        
        const bool ImportSoundtrack(const CharArray &MusicFile)
        {
            ERRORCLASSFUNCTION(BeatProducer,ImportSoundtrack,!MusicFile.IsValid(),RETURN_BOOL)
            FileProcStringTokeniser File;
            
            ERRORCLASSFUNCTION(BeatProducer,ImportSoundtrack,!File.OpenFile(MusicFile,"r"),RETURN_BOOL)
            TemplateArray<TemplateArray<unsigned int> > Temp;
            StringTokeniser MusicMusic;
            StringTokeniser ItemCopy2;
            
            ERRORCLASSFUNCTION(BeatProducer,ImportSoundtrack,!File.CopyToStringTokeniser(MusicMusic,"\n"),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ImportSoundtrack,!Temp.SetSize(MusicMusic.GetNumberOfTokens(),false),RETURN_BOOL)

            ERRORCLASSFUNCTION(BeatProducer,ImportSoundtrack,!ItemCopy2.SetSplitChars(","),RETURN_BOOL)
            do
            {
                ERRORCLASSFUNCTION(BeatProducer,ImportSoundtrack,!ItemCopy2.SetString(MusicMusic.GetCurrString()),RETURN_BOOL)
                ERRORCLASSFUNCTION(BeatProducer,ImportSoundtrack,!ItemCopy2.TokeniseString(),RETURN_BOOL)
                ERRORCLASSFUNCTION(BeatProducer,ImportSoundtrack,!ItemCopy2.ScanToFirstToken(),RETURN_BOOL)
                ERRORCLASSFUNCTION(BeatProducer,ImportSoundtrack,!Temp[MusicMusic.GetTokenNumber()-1].SetSize(ItemCopy2.GetNumberOfTokens(),false),RETURN_BOOL)
                
                do
                {
                    Temp[MusicMusic.GetTokenNumber()-1][ItemCopy2.GetTokenNumber()-1] = StringToLong(ItemCopy2.GetCurrString());
                }while(ItemCopy2.ScanRight());
                
            }while(MusicMusic.ScanRight());
            
            
            Music.TransferFrom(Temp);
            return true;
        }
        
        const bool ExportSoundtrack(const CharArray &MusicFile, const bool Append = false)
        {
            ERRORCLASSFUNCTION(BeatProducer,ExportSoundtrack,!MusicFile.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ExportSoundtrack,!IsValid(),RETURN_BOOL)
            
            CharArray Temp;
            
            SIZE_TYPE Iter = 0, Iter1 = 0;
            
            while(Iter < Music.GetSize())
            {
                Iter1 = 0;
                while(Iter1 < Music[Iter].GetSize())
                {
                    ERRORCLASSFUNCTION(BeatProducer,ExportSoundtrack,!Temp.AppendAll(2,IntToString(Music[Iter][Iter1]).c_str(),","),RETURN_BOOL)
                    Iter1++;
                }
                ERRORCLASSFUNCTION(BeatProducer,ExportSoundtrack,!Temp.Append('\n'),RETURN_BOOL)
                Iter++;
            }
            
            FileProcEquals File;
            if(Append)
            {
                ERRORCLASSFUNCTION(BeatProducer,ExportSoundtrack,!File.OpenFile(MusicFile,"a"),RETURN_BOOL)
            }
            else
            {
                ERRORCLASSFUNCTION(BeatProducer,ExportSoundtrack,!File.OpenFile(MusicFile,"w"),RETURN_BOOL)
            }
            
            ERRORCLASSFUNCTION(BeatProducer,ExportSoundtrack,!File.WriteArray(MusicFile.GetArray(),MusicFile.GetSize()-1),RETURN_BOOL)
            return true;
        }
        
        const bool ExportTrack(const SIZE_TYPE Track, const CharArray &MusicFile, const bool Append = true)
        {
            ERRORCLASSFUNCTION(BeatProducer,ExportTrack,!MusicFile.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ExportTrack,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ExportTrack,Track < 0,RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,ExportTrack,Track > Music.GetSize(),RETURN_BOOL)
            
            CharArray Temp;
            
            
            SIZE_TYPE Iter1 = 0;
            
            while(Iter1 < Music[Track].GetSize())
            {
                ERRORCLASSFUNCTION(BeatProducer,ExportTrack,!Temp.AppendAll(2,IntToString(Music[Track][Iter1]).c_str(),","),RETURN_BOOL)
                Iter1++;
            }
            
            FileProcEquals File;
            if(Append)
            {
                ERRORCLASSFUNCTION(BeatProducer,ExportTrack,!File.OpenFile(MusicFile,"a"),RETURN_BOOL)
            }
            else
            {
                ERRORCLASSFUNCTION(BeatProducer,ExportTrack,!File.OpenFile(MusicFile,"w"),RETURN_BOOL)
            }
            
            ERRORCLASSFUNCTION(BeatProducer,ExportTrack,!File.WriteArray(MusicFile.GetArray(),MusicFile.GetSize()-1),RETURN_BOOL)
            return true;
        }
        //Track specifies which track we're copying from to produce the new mutated track        
        const TemplateArray<unsigned int> ProduceMutatedTrack(const SIZE_TYPE &Track, const SoundBufferLoader &LoadSounds, const int &NumberOfMutations)
        {
            ERRORCLASSFUNCTION(BeatProducer,ProduceMutatedTrack,!Music.IsValid(),return TemplateArray<unsigned int>();)
            ERRORCLASSFUNCTION(BeatProducer,ProduceMutatedTrack,!LoadSounds.IsValid(),return TemplateArray<unsigned int>();)
            ERRORCLASSFUNCTION(BeatProducer,ProduceMutatedTrack,Track < 0,return TemplateArray<unsigned int>();)
            ERRORCLASSFUNCTION(BeatProducer,ProduceMutatedTrack,Track > Music.GetSize()-1,return TemplateArray<unsigned int>();)
            ERRORCLASSFUNCTION(BeatProducer,ProduceMutatedTrack,NumberOfMutations < 1,return TemplateArray<unsigned int>();)
            
            TemplateArray<unsigned int> Temp;
            
            ERRORCLASSFUNCTION(BeatProducer,ProduceMutatedTrack,!Temp.SetArray(Music[Track]),return TemplateArray<unsigned int>();)
            
            int Iter = 0, Iter1 = 0;
            
            while(Iter < NumberOfMutations)
            {
                Iter1 = GetRandomInt(0,Temp.GetSize()-1);
                Temp[Iter1] = GetRandomInt(1,LoadSounds.GetSize());
                Iter++;
            }
            
            return Temp;
        }
        
        const bool InsertMutatedTrack(const SIZE_TYPE &Pos, const SIZE_TYPE &Track, const SoundBufferLoader &LoadSounds, const int &NumberOfMutations)
        {
            TemplateArray<unsigned int> Temp;
            ERRORCLASSFUNCTION(BeatProducer,InsertMutatedTrack,!Temp.SetArray(ProduceMutatedTrack(Track,LoadSounds,NumberOfMutations)),return TemplateArray<unsigned int>();)
            ERRORCLASSFUNCTION(BeatProducer,InsertMutatedTrack,Temp.IsEmpty(),return TemplateArray<unsigned int>();)
            ERRORCLASSFUNCTION(BeatProducer,InsertMutatedTrack,!Music.Insert(Pos,Temp),return TemplateArray<unsigned int>();)
            return true;
        }
        
        //Appends only to the end
        const bool AppendMutatedTrack(const SIZE_TYPE &Track, const SoundBufferLoader &LoadSounds, const int &NumberOfMutations)
        {
            ERRORCLASSFUNCTION(BeatProducer,AppendMutatedTrack,!Music.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,AppendMutatedTrack,!InsertMutatedTrack(Music.GetSize(),Track,LoadSounds,NumberOfMutations),RETURN_BOOL)
            return true;
        }
        
        const bool MutateTrack(const SIZE_TYPE Track, const SoundBufferLoader &LoadSounds, const int NumberOfMutations, const bool DisallowSilence = false)
        {
            ERRORCLASSFUNCTION(BeatProducer,MutateTrack,!Music.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,MutateTrack,!LoadSounds.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,MutateTrack,Track < 0,RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,MutateTrack,Track > Music.GetSize()-1,RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,MutateTrack,NumberOfMutations < 1,RETURN_BOOL)
            
            int Iter = 0, Iter1 = 0;
            
            while(Iter < NumberOfMutations)
            {
                Iter1 = GetRandomInt(0+DisallowSilence,Music[Track].GetSize()-1);
                Music[Track][Iter1] = GetRandomInt(1,LoadSounds.GetSize());
                Iter++;
            }
            
            return true;
        }
        
        const bool MutateBeat(const SoundBufferLoader &LoadSounds, const SIZE_TYPE NumberOfMutationsPerTrack)
        {
            ERRORCLASSFUNCTION(BeatProducer,MutateBeat,!Music.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,MutateBeat,!LoadSounds.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,MutateBeat,NumberOfMutationsPerTrack < 1,RETURN_BOOL)
            
            SIZE_TYPE Track = 0, Iter = 0, Iter1 = 0;
            
            while(Track < Music.GetSize())
            {
                Iter = 0;
                while(Iter < NumberOfMutationsPerTrack)
                {
                    Iter1 = GetRandomInt(0,Music[Track].GetSize()-1);
                    Music[Track][Iter1] = GetRandomInt(1,LoadSounds.GetSize());
                    Iter++;
                }
                
                Track++;
            }
            
            return true;
        }
        
        const bool PlayBeat(const SoundBufferLoader &LoadSounds, const float FastestTempo, const float SlowestTempo, const int NumberOfTracks = 1)
        {
            ERRORFUNCTION(PlayBeat,!Music.IsValid(),RETURN_BOOL)
            ERRORFUNCTION(PlayBeat,!LoadSounds.IsValid(),RETURN_BOOL)
            ERRORFUNCTION(PlayBeat,FastestTempo < 0.1,RETURN_BOOL)
            ERRORFUNCTION(PlayBeat,FastestTempo > SlowestTempo,RETURN_BOOL)
            ERRORFUNCTION(PlayBeat,NumberOfTracks < 1,RETURN_BOOL)
            ERRORFUNCTION(PlayBeat,NumberOfTracks > 10,RETURN_BOOL)
            
            float Tempo = GetRandomFloat(FastestTempo,SlowestTempo);

            SIZE_TYPE Iter = 0;
            
            SoundLoader Sounds[NumberOfTracks];
            
            while(Iter < static_cast<SIZE_TYPE>(NumberOfTracks) )
            {
                ERRORFUNCTION(PlayBeat,!Sounds[Iter].LoadFromList(LoadSounds),RETURN_BOOL)
                Iter++;
            }
            
            Iter = 0;
            SIZE_TYPE Iter1 = 0;
            
            while(Iter1 < Music.GetSize())
            {
                Iter=0;
                while(Iter < Music[Iter1].GetSize())
                {
                    if(Music[Iter1][Iter] > 0)
                    {
                        ERRORCLASSFUNCTION(BeatProducer,PlayBeat,Music[Iter1][Iter] > LoadSounds.GetSize(),RETURN_BOOL)
                        Sounds[Iter%(NumberOfTracks)][Music[Iter1][Iter]].play();
                    }
                    usleep(Tempo*1000);
                    Iter++;
                }
                Iter1++;
            }
            return true;
        }
        
        const bool PlayBeatTrackless(const SoundBufferLoader &LoadSounds, const float FastestTempo, const float SlowestTempo)
        {
            ERRORCLASSFUNCTION(BeatProducer,PlayBeat,!Music.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,PlayBeat,!LoadSounds.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,PlayBeat,FastestTempo < 0.1,RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,PlayBeat,FastestTempo > SlowestTempo,RETURN_BOOL)
            
            float Tempo = GetRandomFloat(FastestTempo,SlowestTempo);

            SIZE_TYPE Iter = 0;
            
            SoundLoader Sound;
            
            ERRORCLASSFUNCTION(BeatProducer,PlayBeat,!Sound.LoadFromList(LoadSounds),RETURN_BOOL)

            SIZE_TYPE Iter1 = 0;
            
            while(Iter1 < Music.GetSize())
            {
                Iter=0;
                while(Iter < Music[Iter1].GetSize())
                {
                    if(Music[Iter1][Iter] > 0)
                    {
                        ERRORCLASSFUNCTION(BeatProducer,PlayBeat,Music[Iter1][Iter] > LoadSounds.GetSize(),RETURN_BOOL)
                        Sound[Music[Iter1][Iter]].play();
                    }
                    usleep(Tempo*1000);
                    Iter++;
                }
                Iter1++;
            }
            return true;
        }
        
        const bool PlayTrackTrackless(const SIZE_TYPE Track,const SoundBufferLoader &LoadSounds, const float FastestTempo, const float SlowestTempo)
        {
            ERRORCLASSFUNCTION(BeatProducer,PlayTrackTrackless,!Music.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,PlayTrackTrackless,Track > Music.GetSize(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,PlayTrackTrackless,!LoadSounds.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,PlayTrackTrackless,FastestTempo < 0.1,RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,PlayTrackTrackless,FastestTempo > SlowestTempo,RETURN_BOOL)
            
            float Tempo = GetRandomFloat(FastestTempo,SlowestTempo);
            
            SoundLoader Sound;
            ERRORCLASSFUNCTION(BeatProducer,PlayTrackTrackless,!Sound.LoadFromList(LoadSounds),RETURN_BOOL)

            SIZE_TYPE Iter = 0;
            
            while(Iter < Music[Track].GetSize())
            {
                ERRORCLASSFUNCTION(BeatProducer,PlayBeat,Music[Track][Iter] > LoadSounds.GetSize(),RETURN_BOOL)
                Sound[Music[Track][Iter]].play();
                usleep(Tempo*1000);
                Iter++;
            }
            return true;
        }
        
        const bool PlayTrack(const int Track,const SoundBufferLoader &LoadSounds, const float FastestTempo, const float SlowestTempo, const int NumberOfTracks = 1)
        {
            ERRORCLASSFUNCTION(BeatProducer,PlayTrack,!Music.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,PlayTrack,static_cast<SIZE_TYPE>(Track) > Music.GetSize(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,PlayTrack,!LoadSounds.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,PlayTrack,FastestTempo < 0.1,RETURN_BOOL)
            ERRORCLASSFUNCTION(BeatProducer,PlayTrack,FastestTempo > SlowestTempo,RETURN_BOOL)
            
            float Tempo = GetRandomFloat(FastestTempo,SlowestTempo);
            
            SIZE_TYPE Iter = 0;
            
            SoundLoader Sounds[NumberOfTracks];
            
            while(Iter < static_cast<SIZE_TYPE>(NumberOfTracks) )
            {
                ERRORFUNCTION(PlayBeat,!Sounds[Iter].LoadFromList(LoadSounds),RETURN_BOOL)
                Iter++;
            }
            
            Iter = 0;
            
            
            while(Iter < Music[Track].GetSize())
            {
                if(Music[Track][Iter] > 0)
                {
                    ERRORCLASSFUNCTION(BeatProducer,PlayBeat,Music[Track][Iter] > LoadSounds.GetSize(),RETURN_BOOL)
                    Sounds[Iter%(NumberOfTracks)][Music[Track][Iter]].play();
                }
                usleep(Tempo*1000);
                Iter++;
            }
            return true;
        }
        
        BeatProducer &operator=(const BeatProducer &ItemCopy)
		{
		    ERRORCLASSFUNCTION(BeatProducer,operator=,!SetBeatProducer(ItemCopy),RETURN_THIS)
			return *this;
		}
		
        BeatProducer &operator=(const TemplateArray<TemplateArray<unsigned int> > &ItemCopy)
		{
		    ERRORCLASSFUNCTION(BeatProducer,operator=,!SetBeatProducer(ItemCopy),RETURN_THIS)
			return *this;
		}
};

const bool ProduceBeat(TemplateArray<TemplateArray<unsigned int> > &Music, const SoundBufferLoader &LoadSounds, const CharArray &Composition, const int BeatsPerPart, const int NumberOfInstruments)
{
    //Check for errors in given infomation
    ERRORFUNCTION(ProduceBeat,!LoadSounds.IsValid(),RETURN_BOOL)
    ERRORFUNCTION(ProduceBeat,!Composition.IsValid(),RETURN_BOOL)
    ERRORFUNCTION(ProduceBeat,NumberOfInstruments < 1,RETURN_BOOL)
    ERRORFUNCTION(ProduceBeat,static_cast<SIZE_TYPE>(NumberOfInstruments) > LoadSounds.GetSize(),RETURN_BOOL)
    
    //Declare variables
    TemplateArray<TemplateArray<unsigned int> > Temp;
    TemplateArray<unsigned int> Instruments;
    TemplateArray<TemplateArray<unsigned int> > CompositionalPiece;
    TemplateArray<unsigned int> CompositionalTime;
    
    StringTokeniser StringToken;
    
    //Figure out how many parts the person wants
    ERRORFUNCTION(ProduceBeat,!StringToken.SetString(Composition),RETURN_BOOL)
    ERRORFUNCTION(ProduceBeat,!StringToken.SetSplitChars(":"),RETURN_BOOL)
    ERRORFUNCTION(ProduceBeat,!StringToken.TokeniseString(),RETURN_BOOL)
    ERRORFUNCTION(ProduceBeat,StringToken.GetNumberOfTokens() < 1,RETURN_BOOL)

    //Set the size of the compositional piece - which will generate one template per part
    //Set the size of compositional time - which determines how many segments each part gets
    ERRORFUNCTION(ProduceBeat,!CompositionalPiece.SetSize(StringToken.GetNumberOfTokens(),false),RETURN_BOOL)
    ERRORFUNCTION(ProduceBeat,!CompositionalTime.SetSize(StringToken.GetNumberOfTokens(),false),RETURN_BOOL)
    
    //Split to check that the numbers are a range
    ERRORFUNCTION(ProduceBeat,!StringToken.SetSplitChars("-"),RETURN_BOOL)
    ERRORFUNCTION(ProduceBeat,!StringToken.TokeniseString(),RETURN_BOOL)
    ERRORFUNCTION(ProduceBeat,IsOdd(StringToken.GetNumberOfTokens()),RETURN_BOOL)
    
    
    
    SIZE_TYPE Total = 0;
    SIZE_TYPE Iter = 0;
    
    //Brackets to ensure X and Y do not last longer than necessary
    {
        ERRORFUNCTION(ProduceBeat,!StringToken.ScanToFirstToken(),RETURN_BOOL)
        int X = 0, Y = 0;
        //Extract the information from the supplied text string on the segments, then randomly pick a number for variation
        while(Iter < CompositionalTime.GetSize())
        {
            X = StringToLong(StringToken.GetCurrString());
            ERRORFUNCTION(ProduceBeat,X < 1,RETURN_BOOL)
            ERRORFUNCTION(ProduceBeat,!StringToken.ScanRight(),RETURN_BOOL)
            Y = StringToLong(StringToken.GetCurrString());
            ERRORFUNCTION(ProduceBeat,Y < 1, RETURN_BOOL)
            
            CompositionalTime[Iter] = GetRandomInt(X,Y);
            
            //Count how many segments there are total
            Total = Total + CompositionalTime[Iter];
            if(!StringToken.ScanRight())
            {
                break;
            }
            Iter++;
        }
    }
    
    //Set our temporary variable to the number of segments
    ERRORFUNCTION(ProduceBeat,!Temp.SetSize(Total),RETURN_BOOL)
    
    //Randomlist stores our list of randomly picked instruments.
    TemplateArray<unsigned int> RandomList;
    
    ERRORFUNCTION(ProduceBeat,!RandomList.SetSize(NumberOfInstruments+1,false),RETURN_BOOL)
    RandomList[0] = 0; //No sound
    
    //Generate the random list of instruments
    Iter = 1;
    SIZE_TYPE Iter1 = 0;
    unsigned int RandomIter = 0;
    
    while(Iter < RandomList.GetSize())
    {
        Iter1 = 0;
        while(IsItem(RandomIter = static_cast<unsigned int>(GetRandomInt(1,LoadSounds.GetSize()) ),RandomList.GetArray(),RandomList.GetSize()))
        {
            //If we're looping for too long searching for a random answer, we step in with a brute force approach
            if(Iter1 > 10)
            {
                if(!IsItem(RandomIter = (Iter1-10),RandomList.GetArray(),RandomList.GetSize()))
                {
                    RandomIter = (Iter1-10);
                    break;
                }
            }
            Iter1++;
        }
        
        
        if(!IsItem(RandomIter,RandomList.GetArray(),RandomList.GetSize()))
        {
            RandomList[Iter] = RandomIter;
            Iter++;
        }
    }
    
    //Generate the compositional piece which will then be used to repeat on intro etc
    
    Iter = 0;
    Iter1 = 0;
    while(Iter < CompositionalPiece.GetSize())
    {
        ERRORFUNCTION(ProduceBeat,!CompositionalPiece[Iter].SetSize(BeatsPerPart,false),RETURN_BOOL)
        
        Iter1 = 0;
        int RandomIter = 0;
        while(Iter1 < static_cast<SIZE_TYPE>(BeatsPerPart))
        {
            RandomIter = GetRandomInt(0,RandomList.GetSize()-1);
            CompositionalPiece[Iter][Iter1] = RandomList[RandomIter];
            ERRORFUNCTION(ProduceBeat,static_cast<SIZE_TYPE>(CompositionalPiece[Iter][Iter1]) > LoadSounds.GetSize(),RETURN_BOOL)
            Iter1++;
        }
        Iter++;
    }
    
    Iter = 0;
    Iter1 = 0;
    SIZE_TYPE Iter2 = 0;

    //Now we have everything to generate the song
    //We count through the compositional time because we need to know how long each segment lasts for
    while(Iter < CompositionalTime.GetSize())
    {
        Iter1 = 0;
        //We then count through the number of segments per section
        while(Iter1 < static_cast<SIZE_TYPE>(CompositionalTime[Iter]))
        {
            //We copy the template compositional piece of this section
            Temp[Iter2].SetArray(CompositionalPiece[Iter]); //This variable (iter) keeps track of the time
            Iter1++; //This variable keeps track of the length of this particular segment
            Iter2++; //This variable keeps track of the total amount
        }
        Iter++;
    }
    //Our music should be generated! Yay... now we leave with it.
    Music.TransferFrom(Temp);
    return true;
}

//NumberOfTracks is how many simultaneous 'threads' will play the sounds in rotation
//For faster beats, ripping can occur as sounds are prematurely stopped to play new ones on the same track
//So add multiple tracks to reduce rip noises. 1 is okay for slow beats, 4 is a safe number
const bool PlayBeat(const TemplateArray<TemplateArray<unsigned int> > &Music, const SoundBufferLoader &LoadSounds, const float FastestTempo, const float SlowestTempo, const int NumberOfTracks = 1)
{
    ERRORFUNCTION(PlayBeat,!Music.IsValid(),RETURN_BOOL)
    ERRORFUNCTION(PlayBeat,!LoadSounds.IsValid(),RETURN_BOOL)
    ERRORFUNCTION(PlayBeat,FastestTempo < 0.1,RETURN_BOOL)
    ERRORFUNCTION(PlayBeat,FastestTempo > SlowestTempo,RETURN_BOOL)
    ERRORFUNCTION(PlayBeat,NumberOfTracks < 1,RETURN_BOOL)
    ERRORFUNCTION(PlayBeat,NumberOfTracks > 10,RETURN_BOOL)
    
    float Tempo = GetRandomFloat(FastestTempo,SlowestTempo);

    SIZE_TYPE Iter = 0;
    
    SoundLoader Sounds[NumberOfTracks];
    
    while(Iter < static_cast<SIZE_TYPE>(NumberOfTracks) )
    {
        ERRORFUNCTION(PlayBeat,!Sounds[Iter].LoadFromList(LoadSounds),RETURN_BOOL)
        Iter++;
    }
    
    Iter = 0;
    SIZE_TYPE Iter1 = 0;
    
    while(Iter1 < Music.GetSize())
    {
        Iter=0;
        while(Iter < Music[Iter1].GetSize())
        {
            if(Music[Iter1][Iter] > 0)
            {
                Sounds[Iter%(NumberOfTracks)][Music[Iter1][Iter]].play();
            }
            usleep(Tempo*1000);
            Iter++;
        }
        Iter1++;
    }
    return true;
}

const bool PlayBeatTrackless(const TemplateArray<TemplateArray<unsigned int> > &Music, const SoundBufferLoader &LoadSounds, const float FastestTempo, const float SlowestTempo)
{
    ERRORFUNCTION(PlayBeat,!Music.IsValid(),RETURN_BOOL)
    ERRORFUNCTION(PlayBeat,!LoadSounds.IsValid(),RETURN_BOOL)
    ERRORFUNCTION(PlayBeat,FastestTempo < 0.1,RETURN_BOOL)
    ERRORFUNCTION(PlayBeat,FastestTempo > SlowestTempo,RETURN_BOOL)
    
    float Tempo = GetRandomFloat(FastestTempo,SlowestTempo);

    SIZE_TYPE Iter = 0;
    
    SoundLoader Sound;
    
    ERRORFUNCTION(PlayBeat,!Sound.LoadFromList(LoadSounds),RETURN_BOOL)

    SIZE_TYPE Iter1 = 0;
    
    while(Iter1 < Music.GetSize())
    {
        Iter=0;
        while(Iter < Music[Iter1].GetSize())
        {
            if(Music[Iter1][Iter] > 0)
            {
                Sound[Music[Iter1][Iter]].play();
            }
            usleep(Tempo*1000);
            Iter++;
        }
        Iter1++;
    }
    return true;
}

TemplateArray<TemplateArray<unsigned int> > ImportSoundtrack(const CharArray &MusicFile)
{
    MusicFile.IsValid();
    FileProcStringTokeniser File;
    File.OpenFile(MusicFile,"r");
    TemplateArray<TemplateArray<unsigned int> > Temp;
    StringTokeniser Music;
    StringTokeniser ItemCopy2;
    
    File.CopyToStringTokeniser(Music,"\n");
    Temp.SetSize(Music.GetNumberOfTokens(),false);

    ItemCopy2.SetSplitChars(",");
    do
    {
        ItemCopy2.SetString(Music.GetCurrString());
        ItemCopy2.TokeniseString();
        ItemCopy2.ScanToFirstToken();
        Temp[Music.GetTokenNumber()-1].SetSize(ItemCopy2.GetNumberOfTokens(),false);
        
        do
        {
            Temp[Music.GetTokenNumber()-1][ItemCopy2.GetTokenNumber()-1] = StringToLong(ItemCopy2.GetCurrString());
        }while(ItemCopy2.ScanRight());
        
    }while(Music.ScanRight());
    
    
    return Temp;
}


const bool RepeatMusicTemplate(TemplateArray<TemplateArray<unsigned int> > &Music, TemplateArray<unsigned int> &TemplateTrack, const int NumberOfTracks)
{
    Music.SetSize(NumberOfTracks,false);
    SIZE_TYPE Iter = 0;
    while(Iter < Music.GetSize())
    {
        Music[Iter].SetArray(TemplateTrack);
        Iter++;
    }
    return true;
}


}

#endif
