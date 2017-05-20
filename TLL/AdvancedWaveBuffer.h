#ifndef G_ADVANCEDWAVEBUFFER_H
#define G_ADVANCEDWAVEBUFFER_H

/*
Generated on: 2015-12-19.05.32

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include "WaveBuffer.h"
#include "Random.h"

namespace TL
{

class AdvancedWaveBuffer : public WaveBuffer
{
    private:
    protected:
    
        const SIZE_TYPE DistortLookup(const CharArray &Type) const
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,DistortLookup,!Type.IsValid(),return 0;)
            if(Compare(Type.GetArray(),Type.GetSize()-1,"Robocise",8,true) )
            {
                return 200;
            }
            else if(Compare(Type.GetArray(),Type.GetSize()-1,"ElectroRobo",11,true) )
            {
                return 400;
            }
            else if(Compare(Type.GetArray(),Type.GetSize()-1,"Computerise",11,true) )
            {
                return 800;
            }
            else if(Compare(Type.GetArray(),Type.GetSize()-1,"Cyborg",6,true) )
            {
                return 1200;
            }
            else if(Compare(Type.GetArray(),Type.GetSize()-1,"Tannoy",6,true) )
            {
                return 2000;
            }
            return 0;
        }
    
    public:
        
        using WaveBuffer::operator=;
        using WaveBuffer::operator==;
        using WaveBuffer::operator!=;
        
        using WaveBuffer::Open;
        using WaveBuffer::Close;
        using WaveBuffer::Reset;
        
        AdvancedWaveBuffer()
        {
            Open();
        }
        
        ~AdvancedWaveBuffer()
        {
            Close();
        }
        
        AdvancedWaveBuffer(const sf::SoundBuffer &ItemCopy)
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,AdvancedWaveBuffer,!SetArray(ItemCopy),)
        }
        
        AdvancedWaveBuffer(const WaveBuffer &ItemCopy)
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,AdvancedWaveBuffer,!SetArray(ItemCopy),)
        }
        
        AdvancedWaveBuffer(const AdvancedWaveBuffer &ItemCopy)
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,AdvancedWaveBuffer,!SetArray(ItemCopy),)
        }
        
        AdvancedWaveBuffer(const CharArray &ItemCopy)
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,AdvancedWaveBuffer,!LoadFromFile(ItemCopy),)
        }
        
        AdvancedWaveBuffer(const TemplateArray<sf::Int16> &ItemCopy, const uint8_t NewNumberOfChannels, const SIZE_TYPE &NewSampleRate)
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,AdvancedWaveBuffer,!SetArray(ItemCopy,NewNumberOfChannels,NewSampleRate),)
        }
        
        AdvancedWaveBuffer(const sf::Int16 ItemCopy[], const SIZE_TYPE Length, const uint8_t NewNumberOfChannels, const SIZE_TYPE &NewSampleRate)
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,AdvancedWaveBuffer,!SetArray(ItemCopy,Length,NewNumberOfChannels,NewSampleRate),)
        }
        
        const bool Reverb(const SIZE_TYPE Delay, const SIZE_TYPE Amount)
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,Reverb,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,Reverb,Delay < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,Reverb,Amount < 1,RETURN_BOOL)
            
            WaveBuffer Copy;
            
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,Reverb,!Copy.SetArray(*this),RETURN_BOOL)
        
            SIZE_TYPE Iter = 0;
            
            CharArray SaveTo;
            
            while(Iter < Amount)
            {
                ERRORCLASSFUNCTION(AdvancedWaveBuffer,Reverb,!Copy.Merge(Delay,Copy),RETURN_BOOL)
                ERRORCLASSFUNCTION(AdvancedWaveBuffer,Reverb,!Copy.Normalise(),RETURN_BOOL)
                Iter++;
            }
        
            TransferFrom(Copy);
            return true;
        }
        
        const bool IrregularReverb(const SIZE_TYPE DelayLowest, const SIZE_TYPE DelayHighest, const SIZE_TYPE Amount)
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,IrregularReverb,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,IrregularReverb,DelayLowest < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,IrregularReverb,DelayHighest < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,IrregularReverb,Amount < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,IrregularReverb,DelayLowest > DelayHighest,RETURN_BOOL)
            
            WaveBuffer Copy;
            
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,IrregularReverb,!Copy.SetArray(*this),RETURN_BOOL)
        
            SIZE_TYPE Iter = 0;
            
            while(Iter < Amount)
            {
                ERRORCLASSFUNCTION(AdvancedWaveBuffer,IrregularReverb,!Copy.Merge(GetRandomInt(DelayLowest,DelayHighest),Copy),RETURN_BOOL)
                ERRORCLASSFUNCTION(AdvancedWaveBuffer,IrregularReverb,!Copy.Normalise(),RETURN_BOOL)
                Iter++;
            }
        
            TransferFrom(Copy);
            return true;
        }
        
        const bool Distort(const CharArray &Type, const SIZE_TYPE Amount)
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,Distort,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,Distort,!Type.IsValid(),RETURN_BOOL)
            
            if(Compare(Type.GetArray(),Type.GetSize()-1,"Creepy",6,true))
            {
                return IrregularReverb(10000,20000,7);
            }
            else
            {
                return Reverb(DistortLookup(Type),Amount);
            }
        }
        
        const bool GenerateRamp(const sf::Int16 Start, const sf::Int16 End, const float HowLong, const SIZE_TYPE StartingFrom, const bool ToMerge = true)
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamp,!IsValidSamplesChannels(),RETURN_BOOL)
            
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamp,HowLong < 0.0,RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamp,StartingFrom < 0,RETURN_BOOL)
            WaveBuffer Temp;
            
            SIZE_TYPE SampleSpacing = SampleRate*HowLong;
            
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamp,SampleSpacing == 0,RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamp,!Temp.SetSize(SampleSpacing*NumberOfChannels,false),RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamp,Temp.GetSize() < 1,RETURN_BOOL)
        
            Temp.SetSampleRate(SampleRate);
            Temp.SetNumberOfChannels(NumberOfChannels);
        
            uint8_t EachChannel = 0;
            SIZE_TYPE I = 0, IR = 0;
            
            while(EachChannel < NumberOfChannels)
            {
                I = EachChannel;
                IR = 0;
                while(I < Temp.GetSize())
                {
                    Temp[I] = ConvertBetweenRanges(Start,End,0,SampleSpacing,IR);
                    I = I + NumberOfChannels;
                    IR++;
                }
                EachChannel++;
            }
            
            if(ToMerge)
            {
                ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamp,!Merge(StartingFrom,Temp),RETURN_BOOL)
            }
            else
            {
                ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamp,!OverWrite(StartingFrom,Temp),RETURN_BOOL)
            }
            return true;
        }
        
        const bool GenerateRamps(const sf::Int16 Start, const sf::Int16 End, const SIZE_TYPE HowMany,const float HowLong, const SIZE_TYPE StartingFrom, const bool ToMerge = true)
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamps,!IsValidSamplesChannels(),RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamps,HowLong < 0.0,RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamps,StartingFrom < 0,RETURN_BOOL)
            
            AdvancedWaveBuffer Temp;
            
            Temp.CopySamplesChannels(*this);
            
            SIZE_TYPE SampleSpacing = SampleRate*HowLong;
            SIZE_TYPE TotalSize = SampleSpacing*HowMany;
            
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamps,SampleSpacing == 0,RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamps,!Temp.SetSize(TotalSize*NumberOfChannels,false),RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamps,Temp.GetSize() < 1,RETURN_BOOL)
            
            uint8_t EachChannel = 0;
            SIZE_TYPE I = 0, IR = 0;
            
            while(EachChannel < NumberOfChannels)
            {
                I = EachChannel;
                IR = 0;
                while(I < Temp.GetSize())
                {
                    if(IR > SampleSpacing){IR = 0;}
                    Temp[I] = ConvertBetweenRanges(Start,End,0,SampleSpacing,IR);
                    I = I + NumberOfChannels;
                    IR++;
                }
                EachChannel++;
            }
            
            if(ToMerge)
            {
                ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamps,!Merge(StartingFrom,Temp),RETURN_BOOL)
            }
            else
            {
                ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRamps,!OverWrite(StartingFrom,Temp),RETURN_BOOL)
            }
            return true;
        }
        
        //This spreads the number of ramps over the given lengths
        const bool GenerateRampsApprox(const sf::Int16 Start, const sf::Int16 End, const SIZE_TYPE HowMany,const float HowLong, const SIZE_TYPE StartingFrom, const bool ToMerge = true)
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRampsApprox,!IsValidSamplesChannels(),RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRampsApprox,HowLong < 0.0,RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRampsApprox,StartingFrom < 0,RETURN_BOOL)
            
            AdvancedWaveBuffer Temp;
            
            Temp.CopySamplesChannels(*this);
            
            SIZE_TYPE SampleSpacing = SampleRate*HowLong;
            SIZE_TYPE RampTotal = SampleSpacing/HowMany;
            
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRampsApprox,RampTotal < 1,RETURN_BOOL)
            
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRampsApprox,SampleSpacing == 0,RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRampsApprox,!Temp.SetSize(SampleSpacing*NumberOfChannels,false),RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRampsApprox,Temp.GetSize() < 1,RETURN_BOOL)
            
            uint8_t EachChannel = 0;
            SIZE_TYPE I = 0, IR = 0;
            
            while(EachChannel < NumberOfChannels)
            {
                I = EachChannel;
                IR = 0;
                while(I < Temp.GetSize())
                {
                    if(IR > RampTotal){IR = 0;}
                    Temp[I] = ConvertBetweenRanges(Start,End,0,RampTotal,IR);
                    I = I + NumberOfChannels;
                    IR++;
                }
                EachChannel++;
            }
            
            if(ToMerge)
            {
                ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRampsApprox,!Merge(StartingFrom,Temp),RETURN_BOOL)
            }
            else
            {
                ERRORCLASSFUNCTION(AdvancedWaveBuffer,GenerateRampsApprox,!OverWrite(StartingFrom,Temp),RETURN_BOOL)
            }
            return true;
        }
        
        const bool SawtoothWave(const sf::Int16 Start, const sf::Int16 End, const float Percent, const float HowLong, const SIZE_TYPE StartingFrom, const bool ToMerge = true)
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,SawtoothWave,!IsValidSamplesChannels(),RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,SawtoothWave,Percent < 0.0,RETURN_BOOL)
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,SawtoothWave,Percent > 100.0,RETURN_BOOL)
            
            SIZE_TYPE SampleSpacing = SampleRate*HowLong;
            SIZE_TYPE HighestSample = SampleSpacing/2;
            SIZE_TYPE RampTotal = (static_cast<float>(HighestSample)/100.0f)*Percent;
            
            return GenerateRampsApprox(Start,End,RampTotal,HowLong,StartingFrom,ToMerge);
        }
        
        const bool SawtoothWave(const float Percent, const float HowLong, const SIZE_TYPE StartingFrom, const bool ToMerge = true)
        {
            return SawtoothWave(-2000,2000,Percent,HowLong,StartingFrom,ToMerge);
        }
        
        const bool RemoveSilenceGaps()
        {
            ERRORCLASSFUNCTION(AdvancedWaveBuffer,RemoveSilenceGaps,!IsValid(),RETURN_BOOL)
            
            uint8_t EachChannel = 0;
            SIZE_TYPE I = 0;
            
            while(EachChannel < NumberOfChannels)
            {
                I = EachChannel;
                
                while(I < Size)
                {
                    if((I-NumberOfChannels) > 0 && (I+NumberOfChannels) < Size)
                    {
                        if(Array[I] == static_cast<sf::Int16>(0))
                        {
                            if(Array[I-NumberOfChannels] != 0 && Array[I-NumberOfChannels] != 0)
                            {
                                //Array[I] = (Array[I-NumberOfChannels] + Array[I-NumberOfChannels])/2;
                                //if(Array[I] == 0){Array[I] = 1;}
                                Array[I] = static_cast<sf::Int16>(1);
                            }
                        }
                    }
                    
                    I = I + NumberOfChannels;
                }
                EachChannel++;
            }
            
            return true;
        }
};

}

#endif
