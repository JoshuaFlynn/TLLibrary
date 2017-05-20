
#ifndef G_RANDOMIMAGEALGORITHM_H
#define G_RANDOMIMAGEALGORITHM_H

/*
Generated on: 2015-11-03-18.25

Version: 3.1
*/

/*
Notes:
Added void versions of int randomisation functions and removed optional arguments
This offers greater function diversity for RandomImage's capabilities

GetRandom changed to GetRandomInt and GetRandomFloat
Added GetRandomInt functions for various computations

SFML discontinued support for srand etc, which have now been replaced by C++ calls
*/

/*
Suggestions for improvements:


*/

#include "Random.h"


namespace TL
{

    uint8_t G_Red_Cycle = 0;
    uint8_t G_Green_Cycle = 0;
    uint8_t G_Blue_Cycle = 0;
    uint8_t G_Hue_Cycle = 0;
    bool Toggle = false;

    void RIAResetAll()
    {
        G_Red_Cycle = 0;
        G_Green_Cycle = 0;
        G_Blue_Cycle = 0;
        G_Hue_Cycle = 0;
        Toggle = false;
    }
    
    void RIARandomiseAll(const int (*RandomAlgorithm)(void))
    {
        G_Red_Cycle = RandomAlgorithm();
        G_Green_Cycle = RandomAlgorithm();
        G_Blue_Cycle = RandomAlgorithm();
        G_Hue_Cycle = RandomAlgorithm();
        Toggle = IsOdd(RandomAlgorithm());
    }
    
    inline const int RIAWonkyHopRed(){ return G_Red_Cycle = (GetRandomAddedInt(G_Red_Cycle)%255); }
    inline const int RIAWonkyHopBlue(){ return G_Blue_Cycle = (GetRandomAddedInt(G_Blue_Cycle)%255); }
    inline const int RIAWonkyHopGreen(){ return G_Green_Cycle = (GetRandomAddedInt(G_Green_Cycle)%255); }
    inline const int RIAWonkyHopHue(){ return G_Hue_Cycle = (GetRandomAddedInt(G_Hue_Cycle)%255); }
    
    inline const int RIACyclePosRed(){ if(G_Red_Cycle == 255){G_Red_Cycle == 0;} return G_Red_Cycle++; }
    inline const int RIACyclePosGreen(){ if(G_Green_Cycle == 255){G_Green_Cycle == 0;} return G_Green_Cycle++; }
    inline const int RIACyclePosBlue(){ if(G_Blue_Cycle == 255){G_Blue_Cycle == 0;} return G_Blue_Cycle++; }
    inline const int RIACyclePosHue(){ if(G_Hue_Cycle == 255){G_Hue_Cycle == 0;} return G_Hue_Cycle++; }
    
    inline const int RIACycleNegRed(){ if(G_Red_Cycle == 0){G_Red_Cycle == 255;} return G_Red_Cycle--; }
    inline const int RIACycleNegGreen(){ if(G_Green_Cycle == 0){G_Green_Cycle == 255;} return G_Green_Cycle--; }
    inline const int RIACycleNegBlue(){ if(G_Blue_Cycle == 0){G_Blue_Cycle == 255;} return G_Blue_Cycle--; }
    inline const int RIACycleNegHue(){ if(G_Hue_Cycle == 0){G_Hue_Cycle == 255;} return G_Hue_Cycle--; }
    
    inline const int RIAPulsedWidthPosRed(){ return (RIACyclePosRed() * (Toggle = !Toggle)); }
    inline const int RIAPulsedWidthPosGreen(){ return (RIACyclePosGreen() * (Toggle = !Toggle)); }
    inline const int RIAPulsedWidthPosBlue(){ return (RIACyclePosBlue() * (Toggle = !Toggle)); }
    inline const int RIAPulsedWidthPosHue(){ return (RIACyclePosHue() * (Toggle = !Toggle)); }
    
    inline const int RIAPulsedWidthToggleRed(){ return (255 * (Toggle = !Toggle)); }
    inline const int RIAPulsedWidthToggleGreen(){ return (255 * (Toggle = !Toggle)); }
    inline const int RIAPulsedWidthToggleBlue(){ return (255 * (Toggle = !Toggle)); }
    inline const int RIAPulsedWidthToggleHue(){ return (255 * (Toggle = !Toggle)); }
    
    inline const int RIAOddHopCyclePosRed()
    {
        int Odd = RIACyclePosRed();
        if(IsOdd(Odd)){return Odd;}
        return 0;
    }
    
    inline const int RIAOddHopCyclePosGreen()
    {
        int Odd = RIACyclePosGreen();
        if(IsOdd(Odd)){return Odd;}
        return 0;
    }
    
    inline const int RIAOddHopCyclePosBlue()
    {
        int Odd = RIACyclePosBlue();
        if(IsOdd(Odd)){return Odd;}
        return 0;
    }
    
    inline const int RIAOddHopCyclePosHue()
    {
        int Odd = RIACyclePosHue();
        if(IsOdd(Odd)){return Odd;}
        return 0;
    }
    
    inline const int RIAEvenHopCyclePosRed()
    {
        int Odd = RIACyclePosRed();
        if(!IsOdd(Odd)){return Odd;}
        return 0;
    }
    
    inline const int RIAEvenHopCyclePosGreen()
    {
        int Odd = RIACyclePosGreen();
        if(!IsOdd(Odd)){return Odd;}
        return 0;
    }
    
    inline const int RIAEvenHopCyclePosBlue()
    {
        int Odd = RIACyclePosBlue();
        if(!IsOdd(Odd)){return Odd;}
        return 0;
    }
    
    inline const int RIAEvenHopCyclePosHue()
    {
        int Odd = RIACyclePosHue();
        if(IsEven(Odd)){return Odd;}
        return 0;
    }
    
    inline const int RIAPulsedWidthOddHopRed(){ return (RIACyclePosRed() * RIAOddHopCyclePosRed()); }
    inline const int RIAPulsedWidthOddHopGreen(){ return (RIACyclePosGreen() * RIAOddHopCyclePosGreen()); }
    inline const int RIAPulsedWidthOddHopBlue(){ return (RIACyclePosBlue() * RIAOddHopCyclePosBlue()); }
    inline const int RIAPulsedWidthOddHopHue(){ return (RIACyclePosHue() * RIAOddHopCyclePosHue()); }
    
    inline const int RIAPulsedWidthEvenHopHue(){ return (RIACyclePosHue() * RIAEvenHopCyclePosHue()); }
    
    
}

#endif
