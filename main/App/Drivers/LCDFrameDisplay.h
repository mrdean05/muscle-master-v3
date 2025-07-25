#ifndef __LCDFRAME_DISPLAY_H__
#define __LCDFRAME_DISPLAY_H__

#include "freertos/FreeRTOS.h"
#include "Spi.h"
#include <string>

namespace APP{

    class LCDFrameDisplay{
        private:
            static SemaphoreHandle_t mutex_;
            static Bsp::SPI* spiDriver_; 
            static Bsp::Logger lcdLogger;
            LCDFrameDisplay();
            static void screenDisplayLock();
            static void screenDisplayUnlock();
            
        public:
            static LCDFrameDisplay* getInstance();
            void initializeLCD( void );
            void launchScreen ( void );
            void launchMenu ( void );
            void launchSalesTable ( void );
            void productDisplay(const Bsp::product_obj_t& item_objt);
            void productDescriptionTextArea(const char key);
            void productDescriptionEnterTextArea(const Bsp::product_obj_t& item_objt);
            void displayTotalPrice(std::string totalPrice);
            void sendInput(const char key);
            void returnTable();
    };


}



#endif