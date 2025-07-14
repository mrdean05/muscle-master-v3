#include "BatHealth.h"

#include <array>

namespace APP {

    BatHealth::BatHealth():max17048I2cBus_(static_cast<uint16_t>(MAX17048_REG::MAX17048_ADDR)){
        
        std::array<uint8_t, 2> versionBuffer;       
        max17048I2cBus_.receiveBuffer(static_cast<uint8_t>(MAX17048_REG::REG_VERSION), versionBuffer);

        //std::array<uint8_t, 3> resetBuffer = { 0xFE, 0x54, 0x00 };
        //max17048I2cBus_.writeBuffer(resetBuffer);

        std::array<uint8_t, 3> qsBuffer = { 0x06, 0x40, 0x00 };
        max17048I2cBus_.writeBuffer(qsBuffer);

    }

    uint16_t BatHealth::getBatterySoc( void ){
        std::array<uint8_t, 2> dataBuffer;
        max17048I2cBus_.receiveBuffer(static_cast<uint8_t>(MAX17048_REG::REG_SOC), dataBuffer);
        uint16_t socValue = ((dataBuffer[0] << 8) | dataBuffer[1]);
        return socValue / 256;
    }


    double BatHealth::getBatteryVoltageLevel( void ){
        std::array<uint8_t, 2> dataBuffer;
        max17048I2cBus_.receiveBuffer(static_cast<uint8_t>(MAX17048_REG::REG_VCELL), dataBuffer);
        uint16_t voltageValue = ((dataBuffer[0] << 8) | dataBuffer[1]);
        
        return (double)voltageValue * 78.125 / 1000000.0;        
    }
}