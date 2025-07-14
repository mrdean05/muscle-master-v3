#ifndef __SPI_H__
#define __SPI_H__
 
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "driver/spi_master.h"
#include "Logger.h"
#include "esp_log.h"
//#include "GpioPin.h"

#include "PinNames.h"
#include "StateEnums.h"

namespace Bsp{
    class SPI {
        private:
            
            //static PinNames spimosi_, spimiso_, spisck_, spics_, spiintr_;
            static Logger spiLogger;
            static SemaphoreHandle_t semaphore_;
            static spi_device_handle_t handle;
            //static GpioPin* spiPinIntr;
            static void clearIntrFlag (void);
            static void IRAM_ATTR gpioIsrHandler(void* arg);
            
        public: 
            SPI();
            static void initialize( PinNames mosi_, PinNames miso_, PinNames sck_, PinNames cs_, PinNames gpioInt_);
            static void waitForScreen(void);
            static void sendSpiData( spiMessagePayload& command );

    };
}

#endif