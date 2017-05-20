#ifndef G_WAVEBUFFER_H
#define G_WAVEBUFFER_H

/*
Generated on: 2015-12-05.04.03

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include <SFML/Audio.hpp>
#include "TemplateArray.h"
#include "Maths.h"

namespace TL
{

const bool LowPassFilter(sf::Int16 ItemCopy[], const SIZE_TYPE Length, const sf::Int16 Limit)
{
    ERRORFUNCTION(LowPassFilter,!ItemCopy,RETURN_BOOL)
    ERRORFUNCTION(LowPassFilter,!Length,RETURN_BOOL)

    int LowEnd = InvertSum(Limit);
    SIZE_TYPE I = 0;
    while(I < Length)
    {
        if(ItemCopy[I] > Limit)
        {
            ItemCopy[I] = Limit;
        }
        else if(ItemCopy[I] < LowEnd)
        {
            ItemCopy[I] = LowEnd;
        }
        
        I++;
    }
    return true;
}

const bool LowPassFilter(sf::Int16 ItemCopy[], const SIZE_TYPE Length, const sf::Int16 SampleRate, const float Decimation)
{
    return LowPassFilter(ItemCopy,Length,SampleRate/Decimation);
}


const bool HighPassFilter(sf::Int16 ItemCopy[], const SIZE_TYPE Length, const sf::Int16 Limit)
{
    ERRORFUNCTION(HighPassFilter,!ItemCopy,RETURN_BOOL)
    ERRORFUNCTION(HighPassFilter,!Length,RETURN_BOOL)

    int LowEnd = InvertSum(Limit);
    SIZE_TYPE I = 0;
    while(I < Length)
    {
        if(ItemCopy[I] > LowEnd && ItemCopy[I] < 0)
        {
            ItemCopy[I] = LowEnd;
        }
        else if(ItemCopy[I] < Limit && ItemCopy[I] > 0)
        {
            ItemCopy[I] = Limit;
        }
        I++;
    }
    return true;
}

const bool BandPass(sf::Int16 ItemCopy[], const SIZE_TYPE Length, const sf::Int16 LimitTop, const sf::Int16 LimitBottom)
{
    ERRORFUNCTION(BandPass,!ItemCopy,RETURN_BOOL)
    ERRORFUNCTION(BandPass,!Length,RETURN_BOOL)

    SIZE_TYPE I = 0;
    while(I < Length)
    {
        if(ItemCopy[I] < LimitBottom)
        {
            ItemCopy[I] = LimitBottom;
        }
        
        if(ItemCopy[I] < LimitTop)
        {
            ItemCopy[I] = LimitTop;
        }
        I++;
    }
    return true;
}

const bool FrequencyPass(sf::Int16 ItemCopy[], const SIZE_TYPE Length, const float FrequencyMultiplier)
{
    ERRORFUNCTION(FrequencyPass,!ItemCopy,RETURN_BOOL)
    ERRORFUNCTION(FrequencyPass,!Length,RETURN_BOOL)

    SIZE_TYPE I = 0;
    while(I < Length)
    {
        ItemCopy[I] = ItemCopy[I] * FrequencyMultiplier;
        I++;
    }
    return true;
}

const bool FrequencyDivider(sf::Int16 ItemCopy[], const SIZE_TYPE Length, const float FrequencyDivide)
{
    ERRORFUNCTION(FrequencyDivider,!ItemCopy,RETURN_BOOL)
    ERRORFUNCTION(FrequencyDivider,!Length,RETURN_BOOL)

    SIZE_TYPE I = 0;
    while(I < Length)
    {
        ItemCopy[I] = ItemCopy[I] / FrequencyDivide;
        I++;
    }
    return true;
}

const bool AverageOut(sf::Int16 ItemCopy[], const SIZE_TYPE Length)
{
    ERRORFUNCTION(AverageOut,!ItemCopy,RETURN_BOOL)
    ERRORFUNCTION(AverageOut,!Length,RETURN_BOOL)

    SIZE_TYPE I = 0;
    while(I < (Length-1))
    {
        ItemCopy[I] = (ItemCopy[I] + ItemCopy[I+1] / 2);
        I++;
    }
    return true;
}

const bool InvertWave(sf::Int16 ItemCopy[], const SIZE_TYPE Length)
{
    ERRORFUNCTION(InvertWave,!ItemCopy,RETURN_BOOL)
    ERRORFUNCTION(InvertWave,!Length,RETURN_BOOL)

    SIZE_TYPE I = 0;
    while(I < Length)
    {
        DEBUGINT(ItemCopy[I])
        DEBUGINT(InvertSum(ItemCopy[I]))
        ItemCopy[I] = InvertSum(ItemCopy[I]);
        I++;
    }
    return true;
}

class WaveBuffer: public TemplateArray<sf::Int16>
{
    private:
    protected:
    
        using TemplateArray<sf::Int16>::OverWrite;
    
        uint8_t NumberOfChannels;
        SIZE_TYPE SampleRate;
        
        void Clear(){NumberOfChannels = 0; SampleRate = 0;}
        
        //How far does the highest/lowest amplitude have to be reduced by to fall under to meet the NyquistRate
        std::pair<float,uint8_t> NyquistReductionRate(const SIZE_TYPE &NewSampleRate, const std::pair<sf::Int16,sf::Int16> &Amplitude) const 
        {
            std::pair<float,uint8_t> RET(0.0,2);
            ERRORCLASSFUNCTION(WaveBuffer,NyquistReductionRate,Amplitude.first == 0 || Amplitude.second == 0,return RET;)
            sf::Int16 Highest = (abs(Amplitude.second) > Amplitude.first) ? abs(Amplitude.second) : Amplitude.first;
            
            if(static_cast<SIZE_TYPE>(Highest) < NewSampleRate){ RET.second = 0; return RET; }
            return std::pair<float,uint8_t>(static_cast<float>(Highest)/static_cast<float>(NewSampleRate),0);
        }
        
        inline void CopySamplesChannels(const SIZE_TYPE &NewSampleRate, const uint8_t &NewNumberOfChannels)
        {
            SampleRate = NewSampleRate;
            NumberOfChannels = NewNumberOfChannels;
        }
        
        inline void CopySamplesChannels(const WaveBuffer &ItemCopy)
        {
            CopySamplesChannels(ItemCopy.SampleRate,ItemCopy.NumberOfChannels);
        }
        
        /*
        void MergeSound(sf::Int16 &Temp, const sf::Int16 *First, const sf::Int16 &Second, const SIZE_TYPE Iter)
        {
            if(First[Iter] == 0)
            {
                if(Iter > NumberOfChannels)
                {
                    if(First[Iter-NumberOfChannels] != 0 || First[Iter+NumberOfChannels] != 0)
                    {
                        Temp = (First[Iter]+Second)/2;
                    }
                    else
                    {
                        Temp = Second;
                    }
                }
                else
                {
                    Temp = (First[Iter]+Second)/2;
                }
            }
            else
            {
                if(Second == 0)
                {
                    Temp = First[Iter];
                }
                else
                {
                    Temp = (First[Iter]+Second)/2;
                }
            }
        }
        */
        
        void MergeSound(sf::Int16 &Temp, const sf::Int16 *First, const sf::Int16 &Second, const SIZE_TYPE Iter)
        {
            Temp = First[Iter] + Second;
        }
        
        
    public:
    
        using TemplateArray<sf::Int16>::operator=;
        
        const bool Normalise()
        {
            ERRORCLASSFUNCTION(WaveBuffer,Normalise,!IsValid(),RETURN_BOOL)
            std::pair<int,int> Nyquist = FindNyquistRate();
            
            float Highest = (Nyquist.first >= InvertSum(Nyquist.second)) ? Nyquist.first : InvertSum(Nyquist.second);
        
            if(Highest < SampleRate){return true;}
        
            SIZE_TYPE Iter = 0;
            float Reduction = ((float)SampleRate)/Highest;
            
            while(Iter < Size)
            {
                Array[Iter] = (Array[Iter] * Reduction);
                Iter++;
            }
            return true;
        }
        
        const bool Smooth()
        {
            ERRORCLASSFUNCTION(WaveBuffer,Smooth,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(WaveBuffer,Smooth,Size < (NumberOfChannels*3),RETURN_BOOL)
            
            uint8_t EachChannel = 0;
            SIZE_TYPE Iter = 0;
            
            while(EachChannel < NumberOfChannels)
            {
                Iter = EachChannel; //Sets offset for reading
                Iter += NumberOfChannels; //We move forward by one
                
                while((Iter+NumberOfChannels) < Size)
                {
                    if(
                    (Array[Iter] >= Array[Iter-NumberOfChannels] && Array[Iter] <= Array[Iter+NumberOfChannels])
                    ||
                    (Array[Iter] <= Array[Iter-NumberOfChannels] && Array[Iter] >= Array[Iter+NumberOfChannels])
                    )
                    {
                        //Do nothing!
                    }
                    else
                    {
                        Array[Iter] = (Array[Iter-NumberOfChannels] + Array[Iter+NumberOfChannels])/2;
                    }
                    
                    Iter += NumberOfChannels;
                }
                EachChannel++;
            }
            
            return true;
        }
    
        void Open()
        {
            Clear();
            TemplateArray<sf::Int16>::Open();
        }
        
        void Close()
        {
            Clear();
            TemplateArray<sf::Int16>::Close();
        }
        
        void Reset()
        {
            Clear();
            TemplateArray<sf::Int16>::Reset();
        }
    
        WaveBuffer(){Open();}
        ~WaveBuffer(){Close();}
        
        void TransferFrom(WaveBuffer &ItemCopy)
        {
            Close();
            
            Array = ItemCopy.Array;
            Size = ItemCopy.Size;
            
            ItemCopy.Array = NULL;
            ItemCopy.Size = 0;
            
            NumberOfChannels = ItemCopy.NumberOfChannels;
            SampleRate = ItemCopy.SampleRate;
            
            ItemCopy.Clear();
            return;
        }
        
        void Swap(WaveBuffer &ItemCopy)
        {
            TemplateArray<sf::Int16>::Swap(ItemCopy);
            WaveBuffer Temp;
            
            Temp.NumberOfChannels = ItemCopy.NumberOfChannels;
            ItemCopy.NumberOfChannels = NumberOfChannels;
            NumberOfChannels = Temp.NumberOfChannels;
            
            Temp.SampleRate = ItemCopy.SampleRate;
            ItemCopy.SampleRate = SampleRate;
            SampleRate = Temp.SampleRate;
            return;
        }
        
        inline void SetNumberOfChannels(const uint8_t NewNumberOfChannels){ NumberOfChannels = NewNumberOfChannels; }
        //This doesn't modify the number of samples recorded. This effectively modifies the speed of playback
        //To modify the recorded samples, choose 'resample'
        inline void SetSampleRate(const SIZE_TYPE NewSampleRate){ SampleRate = NewSampleRate;}
        
        inline const uint8_t GetNumberOfChannels() const { return NumberOfChannels; }
        inline const SIZE_TYPE GetSampleRate() const { return SampleRate; }
        
        const bool SetArray(const sf::Int16 ItemCopy[], const SIZE_TYPE Length, const uint8_t NewNumberOfChannels, const SIZE_TYPE &NewSampleRate)
        {
            ERRORCLASSFUNCTION(WaveBuffer,SetArray,!(TemplateArray<sf::Int16>::SetArray(ItemCopy,Length)),RETURN_BOOL)
            NumberOfChannels = NewNumberOfChannels;
            SampleRate = NewSampleRate;
            return true;
        }
        
        inline const bool SetArray(const TemplateArray<sf::Int16> &ItemCopy, const uint8_t NewNumberOfChannels, const SIZE_TYPE &NewSampleRate)
        {
            return SetArray(ItemCopy.GetArray(),ItemCopy.GetSize(),NewNumberOfChannels,NewSampleRate);
        }
        
        inline const bool SetArray(const WaveBuffer &ItemCopy)
        {
            return SetArray(ItemCopy.GetArray(),ItemCopy.GetSize(),ItemCopy.NumberOfChannels,ItemCopy.SampleRate);
        }
        
        inline const bool SetArray(const sf::SoundBuffer &ItemCopy)
        {
            return SetArray(ItemCopy.getSamples(),ItemCopy.getSampleCount(),ItemCopy.getChannelCount(),ItemCopy.getSampleRate());
        }
        
        inline const bool IsValidSamplesChannels() const
        {
            return (SampleRate != 0 && NumberOfChannels != 0);
        }
        
        inline const bool IsEmptySamplesChannels() const
        {
            return (SampleRate == 0 && NumberOfChannels == 0);
        }
        
        WaveBuffer(const sf::SoundBuffer &ItemCopy)
        {
            ERRORCLASSFUNCTION(WaveBuffer,SoundBuffer,!SetArray(ItemCopy),)
        }
        
        WaveBuffer(const WaveBuffer &ItemCopy)
        {
            ERRORCLASSFUNCTION(WaveBuffer,WaveBuffer,!SetArray(ItemCopy),)
        }
        
        WaveBuffer(const CharArray &ItemCopy)
        {
            ERRORCLASSFUNCTION(WaveBuffer,WaveBuffer,!LoadFromFile(ItemCopy),)
        }
        
        WaveBuffer(const TemplateArray<sf::Int16> &ItemCopy, const uint8_t NewNumberOfChannels, const SIZE_TYPE &NewSampleRate)
        {
            ERRORCLASSFUNCTION(WaveBuffer,WaveBuffer,!SetArray(ItemCopy,NewNumberOfChannels,NewSampleRate),)
        }
        
        WaveBuffer(const sf::Int16 ItemCopy[], const SIZE_TYPE Length, const uint8_t NewNumberOfChannels, const SIZE_TYPE &NewSampleRate)
        {
            ERRORCLASSFUNCTION(WaveBuffer,WaveBuffer,!SetArray(ItemCopy,Length,NewNumberOfChannels,NewSampleRate),)
        }
        
        const bool ConvertToMono()
        {
            ERRORCLASSFUNCTION(WaveBuffer,ConvertToMono,!IsValid(),RETURN_BOOL)
            if(NumberOfChannels == 1){return true;}
            
            WaveBuffer Temp;
            ERRORCLASSFUNCTION(WaveBuffer,ConvertToMono,!Temp.SetSize(Size/NumberOfChannels,false),RETURN_BOOL)
            
            SIZE_TYPE Iter = 0;
            SIZE_TYPE IterChannel = 0;
            uint8_t EachChannel = 0;
            
            while(Iter < Temp.GetSize())
            {
                EachChannel = 0;
                Temp[Iter] = 0;
                while(EachChannel < NumberOfChannels)
                {
                    Temp[Iter] += Array[IterChannel+EachChannel];
                    EachChannel++;
                }
                Iter++;
                IterChannel += NumberOfChannels;
            }
            
            TemplateArray<sf::Int16>::TransferFrom(Temp);
            NumberOfChannels = 1;
            return true;
        }
        
        /*
        const bool ConvertToStereo()
        {
            ERRORCLASSFUNCTION(WaveBuffer,ConvertToStereo,!IsValid(),RETURN_BOOL)
            if(NumberOfChannels == 2){return true;}
            
            WaveBuffer Temp;
            
            ERRORCLASSFUNCTION(WaveBuffer,ConvertToStereo,!Temp.SetSize((Size/NumberOfChannels)*2,false),RETURN_BOOL)
                
            SIZE_TYPE Iter = 0;
                
            while(Iter < Temp.GetSize())
            {
                Temp[Iter]
            }
            
        }
        */
        
        const SIZE_TYPE GetChannelSize() const
        {
            if(Size == 0 || NumberOfChannels == 0){return 0;}
            return Size/NumberOfChannels;
        }
        
        const WaveBuffer GetChannel(const uint8_t Channel) const
        {
            ERRORCLASSFUNCTION(WaveBuffer,GetChannel,!IsValid(),return WaveBuffer();)
            ERRORCLASSFUNCTION(WaveBuffer,GetChannel,Channel < 1,return WaveBuffer();)
            ERRORCLASSFUNCTION(WaveBuffer,GetChannel,Channel > NumberOfChannels,return WaveBuffer();)
            
            WaveBuffer Temp;
            
            ERRORCLASSFUNCTION(WaveBuffer,GetChannel,!Temp.SetSize(Size/NumberOfChannels,false),return WaveBuffer();)
            
            SIZE_TYPE I = 0, I2 = (Channel-1); //Specifies channel offset
            
            while(I < Temp.GetSize())
            {
                Temp[I] = Array[I2];
                I2 = I2 + NumberOfChannels;
                I++;
            }
            
            Temp.NumberOfChannels = 1;
            Temp.SampleRate = SampleRate;
            
            return Temp;
        }
        
        const std::pair<int,int> FindNyquistRate() const
        {
            ERRORCLASSFUNCTION(WaveBuffer,FindNyquistRate,!IsValid(),return (std::pair<int,int>(0,0));)
        
            int Highest = 0, Lowest = 0;
            SIZE_TYPE Iter = 0;
            
            while(Iter < Size)
            {
                if(Array[Iter] > Highest){Highest = Array[Iter];}
                if(Array[Iter] < Lowest){Lowest = Array[Iter];}
                Iter++;
            }
            
            //The Nyquist rate is defined as the highest amplitude times by two
            return std::pair<int,int>(Highest*2,Lowest*2);
        }
        
        const bool Resample(const SIZE_TYPE &NewSampleRate)
        {
            ERRORCLASSFUNCTION(WaveBuffer,Resample,NewSampleRate < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(WaveBuffer,Resample,!IsValid(),RETURN_BOOL)
            
            float Decimation = ((float)SampleRate/(float)NewSampleRate);
            
            SIZE_TYPE ChannelSize = (Size/Decimation)/NumberOfChannels;
            SIZE_TYPE SizeDiff = ChannelSize*NumberOfChannels;
            
            WaveBuffer Temp;
            ERRORCLASSFUNCTION(WaveBuffer,Resample,!Temp.SetSize(SizeDiff),RETURN_BOOL)
            
            uint8_t EachChannel = 0;
            
            Temp.SampleRate = NewSampleRate;
            Temp.NumberOfChannels = NumberOfChannels;
            
            SIZE_TYPE TempIter = 0, AlignmentIter = 0, OriginalIter = 0;
            
            //Iterate through each channel
            while(EachChannel < NumberOfChannels)
            {
                TempIter = EachChannel; //Sets offset
                AlignmentIter = 0;
                while(TempIter < Temp.GetSize())
                {
                    OriginalIter = ConvertBetweenRanges(0,Size/NumberOfChannels,0,Temp.GetSize()/NumberOfChannels,AlignmentIter);
                    
                    Temp[TempIter] = Array[(OriginalIter * NumberOfChannels) + EachChannel];
                    TempIter = TempIter + NumberOfChannels;
                    AlignmentIter++;
                }
                EachChannel++;
            }
            
            TransferFrom(Temp);
            return true;
        }
        
        //Pos doesn't work based on the number of individual samples
        //But the position of the samples relative to the number of channels, so if there's 2 channels
        //It'd look like this:
        //Pos 0 1 2 3 4
        //C1  0 2 4 6 8
        //C2  1 3 5 7 9
        //[The numbers in C1/C2 denote their real positions in the array]
        
        const bool Merge(const SIZE_TYPE Pos, const sf::Int16 ItemCopy[], const SIZE_TYPE Length, const SIZE_TYPE NewSampleRate, const uint8_t NoOfChannels, const bool Smoothing = false)
        {
            ERRORCLASSFUNCTION(WaveBuffer,Merge,!ItemCopy,RETURN_BOOL)
            ERRORCLASSFUNCTION(WaveBuffer,Merge,Length < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(WaveBuffer,Merge,NewSampleRate < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(WaveBuffer,Merge,NoOfChannels < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(WaveBuffer,Merge,Pos < 0,RETURN_BOOL)
            
            WaveBuffer Temp;
            
            SIZE_TYPE ItemCopyChannelSize = Length/NoOfChannels;
            SIZE_TYPE HighestSize = Pos+ItemCopyChannelSize;
            SIZE_TYPE RealPos;
            
            //We check if this has any sound data in it already
            //If not, we simply take on the new properties
            if(IsValid() || (!!SampleRate && !!NumberOfChannels))
            {
                ERRORCLASSFUNCTION(WaveBuffer,Merge,NewSampleRate != SampleRate,RETURN_BOOL)
                ERRORCLASSFUNCTION(WaveBuffer,Merge,NumberOfChannels != NoOfChannels,RETURN_BOOL)
                Temp.NumberOfChannels = NumberOfChannels;
                Temp.SampleRate = SampleRate;
                RealPos = (Pos*NumberOfChannels);
            }
            else
            {
                Temp.NumberOfChannels = NoOfChannels;
                Temp.SampleRate = NewSampleRate;
                RealPos = (Pos*NoOfChannels);
            }                            
            
            SIZE_TYPE RealHighestSize = (RealPos+Length);
            SIZE_TYPE Iter = RealPos, Iter2 = 0;
            
            if(IsValid() && (HighestSize < GetChannelSize()))
            {
                ERRORCLASSFUNCTION(WaveBuffer,Merge,!Temp.SetArray(*this),RETURN_BOOL)
                while(Iter < RealHighestSize)
                {
                    //Temp[Iter] = (Array[Iter] + ItemCopy[Iter2]);
                    MergeSound(Temp[Iter],Array,ItemCopy[Iter2],Iter);
                    Iter2++;
                    Iter++;
                }
            }
            else
            {
                ERRORCLASSFUNCTION(WaveBuffer,Merge,!Temp.SetSize(RealHighestSize,false),RETURN_BOOL)
                
                //If the position of the new sound 'jumps' a gap, we set that gap to silence
                
                if(RealPos > Size)
                {
                    Iter = Size;
                    while(Iter < RealPos)
                    {
                        Temp[Iter] = 0;
                        Iter++;
                    }
                    Iter = RealPos;
                }
                
                if(IsValid())
                {
                    ERRORCLASSFUNCTION(WaveBuffer,Merge,!Temp.OverWrite(0,Array,Size),RETURN_BOOL)
                }
                
                while(Iter < Temp.GetSize())
                {
                
                    if(Iter < Size)
                    {
                        //Temp[Iter] = (Array[Iter] + ItemCopy[Iter2]);
                        MergeSound(Temp[Iter],Array,ItemCopy[Iter2],Iter);
                    }
                    else
                    {
                        Temp[Iter] = ItemCopy[Iter2];
                    }
                
                    Iter++;
                    Iter2++;
                }
                
                
            }
            
            //Temp.Normalise();
            if(Smoothing){Temp.Smooth();}
            TransferFrom(Temp);
            
            return true;
        }
        
        const bool Merge(const SIZE_TYPE Pos, const WaveBuffer &ItemCopy)
        {
            return Merge(Pos,ItemCopy.GetArray(),ItemCopy.GetSize(),ItemCopy.GetSampleRate(),ItemCopy.GetNumberOfChannels());
        }
        
        const bool Merge(const SIZE_TYPE Pos, const sf::SoundBuffer &ItemCopy)
        {
            return Merge(Pos,ItemCopy.getSamples(),ItemCopy.getSampleCount(),ItemCopy.getSampleRate(),ItemCopy.getChannelCount());
        }
        
        const bool MergeSilence(const SIZE_TYPE Pos, const SIZE_TYPE Length, const SIZE_TYPE NewSampleRate, const uint8_t NoOfChannels)
        {
            ERRORCLASSFUNCTION(WaveBuffer,MergeSilence,Length < 1,RETURN_BOOL)
            if(!!SampleRate)
            {
                ERRORCLASSFUNCTION(WaveBuffer,MergeSilence,SampleRate != NewSampleRate,RETURN_BOOL)
            }
            
            if(!!NumberOfChannels)
            {
                ERRORCLASSFUNCTION(WaveBuffer,MergeSilence,NumberOfChannels != NoOfChannels,RETURN_BOOL)
            }
            
            TemplateArray<sf::Int16> Temp;
            
            ERRORCLASSFUNCTION(WaveBuffer,MergeSilence,!Temp.SetSize(Length*NoOfChannels),RETURN_BOOL)
            
            SIZE_TYPE Iter = 0;
            
            while(Iter < Temp.GetSize())
            {
                Temp[Iter] = 0;
                Iter++;
            }
            
            return Merge(Pos,Temp.GetArray(),Length,NewSampleRate,NoOfChannels);
        }
        
        const bool MergeSilence(const SIZE_TYPE Pos, const SIZE_TYPE Length)
        {
            if(!IsValid())
            {
                ERRORCLASSFUNCTION(WaveBuffer,MergeSilence,!(NumberOfChannels != 0 && SampleRate > 0),RETURN_BOOL)
            }
            return MergeSilence(Pos,Length,SampleRate,NumberOfChannels);
        }
        
        const bool OverWrite(const SIZE_TYPE Pos, const sf::Int16 ItemCopy[], const SIZE_TYPE Length, const SIZE_TYPE NewSampleRate, const uint8_t NoOfChannels)
        {
            ERRORCLASSFUNCTION(WaveBuffer,OverWrite,!ItemCopy,RETURN_BOOL)
            ERRORCLASSFUNCTION(WaveBuffer,OverWrite,Length < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(WaveBuffer,OverWrite,NewSampleRate < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(WaveBuffer,OverWrite,NoOfChannels < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(WaveBuffer,OverWrite,Pos < 0,RETURN_BOOL)
            
            WaveBuffer Temp;
            SIZE_TYPE RealPos;
            
            if(IsValid())
            {
                ERRORCLASSFUNCTION(WaveBuffer,OverWrite,NewSampleRate != SampleRate,RETURN_BOOL)
                ERRORCLASSFUNCTION(WaveBuffer,OverWrite,NumberOfChannels != NoOfChannels,RETURN_BOOL)
                Temp.NumberOfChannels = NumberOfChannels;
                Temp.SampleRate = SampleRate;
                RealPos = (Pos*NumberOfChannels);
            }
            else
            {
                Temp.NumberOfChannels = NoOfChannels;
                Temp.SampleRate = NewSampleRate;
                RealPos = (Pos*NoOfChannels);
            }
            
            SIZE_TYPE RealHighestSize = (RealPos+Length);
            SIZE_TYPE HighestSize = Pos+(Length/NoOfChannels);
            
            if(IsValid() && (HighestSize < GetChannelSize()))
            {
                ERRORCLASSFUNCTION(WaveBuffer,OverWrite,!Temp.SetArray(*this),RETURN_BOOL)
            }
            else
            {
                ERRORCLASSFUNCTION(WaveBuffer,OverWrite,!Temp.SetSize(RealHighestSize,false),RETURN_BOOL)
            }
            
            SIZE_TYPE Iter = RealPos, Iter2 = 0;
            
            while(Iter < RealHighestSize)
            {
                Temp[Iter] = ItemCopy[Iter2];
                Iter++;
                Iter2++;
            }
            
            TransferFrom(Temp);
            return true;
        }
        
        const bool OverWrite(const SIZE_TYPE Pos, const WaveBuffer &ItemCopy)
        {
            return OverWrite(Pos,ItemCopy.GetArray(),ItemCopy.GetSize(),ItemCopy.GetSampleRate(),ItemCopy.GetNumberOfChannels());
        }
        
        const bool OverWrite(const SIZE_TYPE Pos, const sf::SoundBuffer &ItemCopy)
        {
            return OverWrite(Pos,ItemCopy.getSamples(),ItemCopy.getSampleCount(),ItemCopy.getSampleRate(),ItemCopy.getChannelCount());
        }
        
        const bool ToSfSoundBuffer(sf::SoundBuffer &ItemCopy)
        {
            ERRORCLASSFUNCTION(WaveBuffer,ToSfSoundBuffer,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(WaveBuffer,ToSfSoundBuffer,!ItemCopy.loadFromSamples(Array,Size,NumberOfChannels,SampleRate),RETURN_BOOL)
            return true;
        }
        
        //Note, this MUST include the extension in the filename
        const bool SaveToFile(const CharArray &Filename)
        {
            ERRORCLASSFUNCTION(WaveBuffer,SaveToFile,!IsValid(),RETURN_BOOL)
            
            sf::SoundBuffer ItemCopy;
            
            ERRORCLASSFUNCTION(WaveBuffer,SaveToFile,!ItemCopy.loadFromSamples(Array,Size,NumberOfChannels,SampleRate),RETURN_BOOL)
            ERRORCLASSFUNCTION(WaveBuffer,SaveToFile,!ItemCopy.saveToFile(Filename.GetArray()),RETURN_BOOL)
            return true;
        }
        
        const bool LoadFromFile(const CharArray &Filename)
        {
            sf::SoundBuffer ItemCopy;
            
            ERRORCLASSFUNCTION(WaveBuffer,LoadFromFile,!ItemCopy.loadFromFile(Filename.GetArray()),RETURN_BOOL)
            ERRORCLASSFUNCTION(WaveBuffer,LoadFromFile,!SetArray(ItemCopy),RETURN_BOOL)
            return true;
        }
        
        //This should only be used to 'demo' or check a sound, and not mainly used for actual playing
        //as it makes use of 'busy waiting' when this really should be outsourced to an external
        //multi-threaded manager
        const bool Play()
        {
            ERRORCLASSFUNCTION(WaveBuffer,Play,!IsValid(),RETURN_BOOL)
            
            sf::SoundBuffer ItemCopy;
            
            ERRORCLASSFUNCTION(WaveBuffer,Play,!ItemCopy.loadFromSamples(Array,Size,NumberOfChannels,SampleRate),RETURN_BOOL)
            
            sf::Sound PlayThis;
            
            PlayThis.setBuffer(ItemCopy);
            PlayThis.play();
            
            while(PlayThis.getStatus() == sf::Sound::Playing);
            return true;
        }

        const bool IsValid() const { return NumberOfChannels > 0 && SampleRate > 0 && TemplateArray<sf::Int16>::IsValid(); }
        const bool IsEmpty() const { return NumberOfChannels == 0 && SampleRate == 0 && TemplateArray<sf::Int16>::IsEmpty(); }

        const bool IsCompatibleSoundBuffer(const sf::SoundBuffer &ItemCopy) const
        {
            if(ItemCopy.getChannelCount() != NumberOfChannels){return false;}
            if(ItemCopy.getSampleRate() != SampleRate){return false;}
            return true;
        }
        
        const bool IsCompatibleSoundBuffer(const WaveBuffer &ItemCopy) const
        {
            if(ItemCopy.IsValid() != IsValid()){return false;}
            if(ItemCopy.NumberOfChannels != NumberOfChannels){return false;}
            if(ItemCopy.SampleRate != SampleRate){return false;}
            return true;
        }

        inline WaveBuffer &operator=(const WaveBuffer &ItemCopy)
        {
            ERRORCLASSFUNCTION(WaveBuffer,operator=,!SetArray(ItemCopy),RETURN_THIS)
            return *this;
        }
        
        inline WaveBuffer &operator=(const sf::SoundBuffer &ItemCopy)
        {
            ERRORCLASSFUNCTION(WaveBuffer,operator=,!SetArray(ItemCopy),RETURN_THIS)
            return *this;
        }
        
        inline const bool operator==(const WaveBuffer &ItemCopy) const
        {
            return TemplateArray<sf::Int16>::operator==(ItemCopy) && ItemCopy.NumberOfChannels == NumberOfChannels && ItemCopy.SampleRate == SampleRate;
        }
        
        inline const bool operator!=(const WaveBuffer &ItemCopy) const
        {
            return !(ItemCopy == *this);
        }
};

}

#endif

