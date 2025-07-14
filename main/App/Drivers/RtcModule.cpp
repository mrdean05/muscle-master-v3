#include <array>

#include "RtcModule.h"


namespace APP {

    Bsp::I2c RtcModule::pcf8523I2cBus(static_cast<uint16_t>(PCF8523_REG::ADDR));
    RtcModule::RtcModule()
    {
        // Set Control_1 register: normal mode, no clock output
        std::array<uint8_t, 2> normalModeBuffer = {static_cast<uint8_t>(PCF8523_REG::CONTROL_1), 0x00};
        pcf8523I2cBus.writeBuffer(normalModeBuffer);

        // Set Control_2 register: disable watchdog
        std::array<uint8_t, 2> controlBuffer = {static_cast<uint8_t>(PCF8523_REG::CONTROL_2), 0x00};
        pcf8523I2cBus.writeBuffer(controlBuffer);

        controlBuffer = {static_cast<uint8_t>(PCF8523_REG::CONTROL_3), 0x00};
        pcf8523I2cBus.writeBuffer(controlBuffer);

        dateAndTime timeData = {};
        timeData.seconds = 30;
        timeData.minutes = 20;
        timeData.hours   = 4;
        setTime(timeData);
    }


    void RtcModule::setTime( dateAndTime& timeDate ){
        
        // Convert to BCD format
        uint8_t bcd_hours   = ((timeDate.hours / 10) << 4 | (timeDate.hours / 10));
        uint8_t bcd_minutes = ((timeDate.minutes / 10) << 4) | (timeDate.minutes % 10);
        uint8_t bcd_seconds = ((timeDate.seconds / 10) << 4) | (timeDate.seconds % 10);

        std::array<uint8_t, 2> timeBuffer = {static_cast<uint8_t>(PCF8523_REG::SECONDS), bcd_seconds};
        pcf8523I2cBus.writeBuffer(timeBuffer);

        timeBuffer = {static_cast<uint8_t>(PCF8523_REG::MINUTES), bcd_minutes};
        pcf8523I2cBus.writeBuffer(timeBuffer);

        timeBuffer = {static_cast<uint8_t>(PCF8523_REG::HOURS), bcd_hours};
        pcf8523I2cBus.writeBuffer(timeBuffer);        
    }

    RtcModule::dateAndTime RtcModule::getTime ( void ){

        dateAndTime time;
        std::array<uint8_t, 1> dataBuffer;
        pcf8523I2cBus.receiveBuffer(static_cast<uint8_t>(PCF8523_REG::SECONDS), dataBuffer);
        time.seconds = ((dataBuffer[0] >> 4) * 10) + (dataBuffer[0] & 0x0F);

        pcf8523I2cBus.receiveBuffer(static_cast<uint8_t>(PCF8523_REG::MINUTES), dataBuffer);
        time.minutes = ((dataBuffer[0] >> 4) * 10) + (dataBuffer[0] & 0x0F);  
        
        pcf8523I2cBus.receiveBuffer(static_cast<uint8_t>(PCF8523_REG::HOURS), dataBuffer);
        time.hours = ((dataBuffer[0] >> 4) * 10) + (dataBuffer[0] & 0x0F);    
        
        return time;
    }


}
