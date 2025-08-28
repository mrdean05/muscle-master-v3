#include "Board.h"

#include "esp_mac.h"  

namespace Bsp {
    
    uint8_t Board::macAddr[6] = { 0 };
  
    Board::Board(){
        esp_read_mac(macAddr, ESP_MAC_WIFI_STA); 
    }

    const char* Board::getMacAddr(){

        static char macAddrStr[13];
        snprintf(macAddrStr, sizeof(macAddrStr),
                "%02X%02X%02X%02X%02X%02X",
                macAddr[0], macAddr[1], macAddr[2],
                macAddr[3], macAddr[4], macAddr[5]);

        return macAddrStr;
        
    }

}