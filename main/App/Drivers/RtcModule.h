#ifndef __RTC_MODULE_H__
#define __RTC_MODULE_H__

#include <cstdint>
#include "I2c.h"

namespace APP{
    class RtcModule{
        private:
            enum class PCF8523_REG : uint8_t {
                ADDR         =      0x68,
                CONTROL_1    =      0x00,
                CONTROL_2    =      0x01,
                CONTROL_3    =      0x02,
                SECONDS      =      0x03,
                MINUTES      =      0x04,
                HOURS        =      0x05,
                DAYS         =      0x06,
                WEEKDAYS     =      0x07,
                MONTHS       =      0x08,
                YEARS        =      0x09
            };

        static Bsp::I2c pcf8523I2cBus;

        public:
            struct dateAndTime {
                uint8_t hours;
                uint8_t minutes;
                uint8_t seconds;
            };
            RtcModule();
            static void setTime( dateAndTime& timeDate );
            static dateAndTime getTime ( void );
    };
}


#endif