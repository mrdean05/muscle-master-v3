#include "KeypadCharacter.h"

namespace APP{

    char KeypadCharacter::ch = '\0';

    //To function to reset the keypad and also interrupt

    #define ROW_DATA                            0x1F       // 4 rows: 00001111
    #define COL_DATA                            0x7F       // 4 cols: 00001111



    KeypadCharacter::KeypadCharacter()
        :i2cKeypad_(static_cast<uint16_t>(TCA8418_MATRIX_REG::ADDR)){
        
        /* Setting up the rows as 4 */
        /*  register and data*/
        std::array<uint8_t, 2> setRowBuffer = {static_cast<uint8_t>(TCA8418_MATRIX_REG::KP_GPIO1_ADDR), ROW_DATA};
        i2cKeypad_.writeBuffer(setRowBuffer);

        /* Setting up the cols as 4 */
        std::array<uint8_t, 2> setColBuffer = {static_cast<uint8_t>(TCA8418_MATRIX_REG::KP_GPIO2_ADDR), COL_DATA};
        i2cKeypad_.writeBuffer(setColBuffer);

        /* Setting up the colums (8,9) as 0 */
        std::array<uint8_t, 2> setCol89Buffer = {static_cast<uint8_t>(TCA8418_MATRIX_REG::KP_GPIO3_ADDR), 0};
        i2cKeypad_.writeBuffer(setCol89Buffer);

        /* Clear interupt */
        std::array<uint8_t, 2> clearIntrBuf = {static_cast<uint8_t>(TCA8418_MATRIX_REG::INT_STAT_REG_ADDR), 0x01};
        i2cKeypad_.writeBuffer(clearIntrBuf);

    }

    KeypadCharacter* KeypadCharacter::getInstance(){
        static KeypadCharacter keyChar;
        return &keyChar;
    }

    uint8_t KeypadCharacter::processKeyPressed(){
        std::array<uint8_t, 2> buffer = {0 , 0};
        std::array<uint8_t, 1> keyData = { 0 };

        std::array<uint8_t, 2> clearIntrBuf = {static_cast<uint8_t>(TCA8418_MATRIX_REG::INT_STAT_REG_ADDR), 0x01};

        i2cKeypad_.receiveBuffer(static_cast<uint8_t>(TCA8418_MATRIX_REG::INT_STAT_REG_ADDR), buffer);
        if (buffer[0] == 1){ 
            /* Release the key status (release or press) & key type - 7 bit shows key status*/
            i2cKeypad_.receiveBuffer(static_cast<uint8_t>(TCA8418_MATRIX_REG::KEY_EVENT_A_REG_ADDR), keyData);
            if ((0x80 & keyData[0]) == 0){
                keyData[0] = 0x7F & keyData[0];
                
                i2cKeypad_.writeBuffer(clearIntrBuf);
                return keyData[0];
            }
            i2cKeypad_.writeBuffer(clearIntrBuf);
        }
        return 255;
    }

    char KeypadCharacter::getPressedKey(void){
        uint8_t keyPressed = processKeyPressed();
        switch(keyPressed){
            case 6: ch = '^'; break;      //
            case 7: ch = '>'; break;
            case 11: ch = 'M'; break;     // M: MENU
            case 12: ch = '7'; break;
            case 13: ch = '8'; break;
            case 14: ch = '9'; break;
            case 15: ch = 'W'; break;     // W: M+
            case 16: ch = '<'; break;     //
            case 17: ch = 'D'; break;     // D: down
            case 21: ch = 'S'; break;     // S: SHIFT
            case 22: ch = '4'; break;
            case 23: ch = '5'; break;
            case 24: ch = '6'; break;     // 
            case 25: ch = 'Q'; break;     // Q: M-  
            case 26: ch = 'E'; break;     // E: MC
            case 27: ch = 'I'; break;     // I: CASH IN
            case 31: ch = 'A'; break;     // A: ALL CLEAR - AC
            case 32: ch = '1'; break;     //
            case 33: ch = '2'; break;      //
            case 34: ch = '3'; break;     //
            case 35: ch = 'X'; break;     // X
            case 36: ch = 'O'; break;     // O: CASH OUT
            case 37: ch = '/'; break;     //
            case 41: ch = 'C'; break;     // C: CLEAR 
            case 42: ch = '0'; break;     //
            case 43: ch = 'P'; break;     // P: 00 or space
            case 44: ch = '.'; break;     //
            case 45: ch = '+'; break;     //
            case 46: ch = '-'; break;     // 
            case 47: ch = '='; break;     // = : = or ENTER
            default: ch = '\0'; break;             
        }
        return ch;
    }
}