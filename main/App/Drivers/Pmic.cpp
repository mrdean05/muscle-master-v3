#include "pmic.h"

namespace APP {
    Pmic::Pmic():pmicI2cBus_(static_cast<uint16_t>(BQ24296_REG::ADDR)){}

    uint8_t Pmic::getPowerOnConfig(){
        std::array<uint8_t, 1> powerOnBuffer;
        pmicI2cBus_.receiveBuffer(static_cast<uint8_t>(BQ24296_REG::REG01), powerOnBuffer);
        return powerOnBuffer[0];
    }

    uint8_t Pmic::getSystemStatus(){
        std::array<uint8_t, 1> systemStatusBuffer;
        pmicI2cBus_.receiveBuffer(static_cast<uint8_t>(BQ24296_REG::REG08), systemStatusBuffer);
        return systemStatusBuffer[0];        
    }

    uint8_t Pmic::getFaultStatus(){
        std::array<uint8_t, 1> faultBuffer;
        pmicI2cBus_.receiveBuffer(static_cast<uint8_t>(BQ24296_REG::REG09), faultBuffer);
        return faultBuffer[0];   
    }
}