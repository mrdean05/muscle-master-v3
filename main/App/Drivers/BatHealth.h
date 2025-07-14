#ifndef __BAT_HEALTH_H__
#define __BAT_HEALTH_H__

#include <cstdint>

#include "I2c.h"

namespace APP {
    class BatHealth {
        
        private:
            enum class MAX17048_REG: uint8_t {
                MAX17048_ADDR   =   0x36,
                REG_VCELL       =   0x02,
                REG_SOC         =   0x04,
                REG_VERSION     =   0x08
            };
        
            Bsp::I2c max17048I2cBus_;
        
        public:
            BatHealth();
            uint16_t getBatterySoc( void );
            double getBatteryVoltageLevel( void );

    };
}



#endif