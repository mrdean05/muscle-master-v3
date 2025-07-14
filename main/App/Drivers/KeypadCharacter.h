#ifndef __KEYPADCHARACTER_H__
#define __KEYPADCHARACTER_H__

#include "I2c.h"

namespace APP{
    class KeypadCharacter{
        private:
            enum class TCA8418_MATRIX_REG : uint8_t {
                ADDR     =   0x34,
                CONFIG_REG_ADDR         =   0x01,
                KP_GPIO1_ADDR           =   0x1D,
                KP_GPIO2_ADDR           =   0x1E,
                KP_GPIO3_ADDR           =   0x1F,
                ROW_DATA                =   0x1F,       // 4 rows: 00001111
                COL_DATA                =   0x7F,       // 4 cols: 00001111
                COL_ANNEX_DATA          =   0x00,       
                INT_STAT_REG_ADDR       =   0x02,       // interrupt status register
                KEY_LCK_EC_REG_ADDR     =   0x03,       // Key Lock and Event Counter Register
                KEY_EVENT_A_REG_ADDR    =   0x04   
            };
            static char ch;
            Bsp::I2c i2cKeypad_;
            KeypadCharacter();
            uint8_t processKeyPressed();

        public:
            static KeypadCharacter* getInstance();
            char getPressedKey(void);
            static char getPressedKey2(void) { return ch; }
    };
}


#endif