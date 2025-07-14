#ifndef __PMIC_H__
#define __PMIC_H__

#include "I2c.h"

namespace APP {
    class Pmic {
        private:
            enum class BQ24296_REG : uint8_t {
                ADDR     =   0x6B,
                REG00    =   0x00,
                REG01    =   0x01,
                REG02    =   0x02,
                REG03    =   0x03,
                REG07    =   0x07,
                REG08    =   0x08,
                REG09    =   0x09     
            };
            Bsp::I2c pmicI2cBus_;

        public:
            Pmic();  
            uint8_t getPowerOnConfig();
            uint8_t getSystemStatus();
            uint8_t getFaultStatus(); 
    };                 
}


#endif