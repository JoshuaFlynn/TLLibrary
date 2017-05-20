#ifndef G_ADVANCEDCLOCK_H
#define G_ADVANCEDCLOCK_H

/*
Generated on: 2012-08-08-20.35.22

Version: 1.3
*/

/*
Notes:
Expanded operator functionality, as well as the capacity to copy clocks

*/

/*
Suggestions for improvements:


*/

//Not supported on linux
//#include <SFML/System.hpp>
//sf::priv::Platform::GetSystemTime()
#include <ctime>
#include <unistd.h> //usleep. Might have to convert to a suitable version for other OSes. Use an IF/ELSE define


#define USLEEP usleep(1)
#define SYSTEM_TIME clock()

#ifndef USEC_TO_SEC
#define USEC_TO_SEC 1000
#endif

namespace TL
{

class AdvancedClock
{
    protected:

        bool IsPaused;
        double StartTime;
        double PauseTime;
        
        void Clear(){ StartTime = 0.0; PauseTime = 0; IsPaused = false; }

    public:
        void Open(){Clear();}
        void Close(){Clear();}
        
        static double usecdiff; //Difference in ticks to one usleep microsecond
        
        const bool IsRunning() const {return (StartTime > 0.0) && !IsPaused;}
        const bool IsValid() const {return (StartTime > 0.0);}
        const bool IsEmpty() const {return (StartTime == 0.0 && PauseTime == 0 && IsPaused == false);}

        void SetAdvancedClock(const AdvancedClock &ItemCopy)
        {
            IsPaused = ItemCopy.IsPaused;
            StartTime = ItemCopy.StartTime;
            PauseTime = ItemCopy.PauseTime;
        }

        void Reset(){Open();}
        void Start()
        {
            Reset();
            if(usecdiff <= 0.0)
            {
                //usecdiff has not been set, so we configure it:
                StartTime = SYSTEM_TIME;
                USLEEP;
                usecdiff = SYSTEM_TIME-StartTime;
            }
            
            StartTime = (SYSTEM_TIME/usecdiff)/USEC_TO_SEC;
        }
        void Pause()
        {
            if(!IsPaused)
            {
                IsPaused = true; PauseTime = (SYSTEM_TIME/usecdiff)/USEC_TO_SEC;
            }
        }

        void Unpause()
        {
            if(IsPaused)
            {
                StartTime =  ((SYSTEM_TIME/usecdiff)/USEC_TO_SEC) - (PauseTime-StartTime);
                //StartTime = (SYSTEM_TIME-(PauseTime-StartTime));
            }
            PauseTime = 0;
            IsPaused = false;
        }

        const float GetElapsedTime() const
        {
            if(IsPaused)
            {
                return static_cast<float>(PauseTime-StartTime);
            }
            return static_cast<float>(((SYSTEM_TIME/usecdiff)/USEC_TO_SEC)-StartTime);
        }

		AdvancedClock(){Open();}
		AdvancedClock(const AdvancedClock &ItemCopy)
		{
		    SetAdvancedClock(ItemCopy);
		}
		~AdvancedClock(){Close();}

        AdvancedClock &operator=(const AdvancedClock &ItemCopy)
        {
            SetAdvancedClock(ItemCopy);
            return *this;
        }

        const bool operator==(const AdvancedClock &ItemCopy) const
        {
            if(IsPaused != ItemCopy.IsPaused){return false;}
            if(StartTime != ItemCopy.StartTime){return false;}
            if(PauseTime != ItemCopy.PauseTime){return false;}
            return true;
        }

        const bool operator!=(const AdvancedClock &ItemCopy) const
        {
            return !(*this == ItemCopy);
        }
};

double AdvancedClock::usecdiff = 0.0;

}

#endif
