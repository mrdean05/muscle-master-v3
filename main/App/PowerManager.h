#ifndef __POWER_MANAGER_H__
#define __POWER_MANAGER_H__

#include "GpioPin.h"

namespace APP{
    class PowerManager{
        private:
            Bsp::GpioPin psHold;
            Bsp::GpioPin pbOut;
            Bsp::GpioPin stm6600Int;

            bool triggerOffState = false;

            void triggerPowerOffCalback(void);
        
        public:
            PowerManager();
            void setPowerOn( void );
            void setPowerOff( void );
            bool getPushButtonOutState( void );
            bool getTriggerOffState( void );
    };
}


#endif