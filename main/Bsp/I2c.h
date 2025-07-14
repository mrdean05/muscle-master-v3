#ifndef __I2C_H__
#define __I2C_H__

#include <array>

#include "freertos/FreeRTOS.h"
#include "driver/i2c_master.h"
#include "esp_log.h"

//ToDo, make the read and writing of the transmit tied to the class -make it static

namespace Bsp{
    class I2c {
        private:
            static SemaphoreHandle_t mutex_;
            static i2c_master_bus_handle_t bus_handle;
            i2c_master_dev_handle_t dev_handle;

            static bool isBusInitialized; 
            static void initializeBus(void);
            uint16_t deviceAddr_;

        public:
            explicit I2c(uint16_t deviceAddr);
            template<std::size_t N>
            void writeBuffer(std::array<uint8_t, N>& buffer, TickType_t timeout_ticks = 3000/portTICK_PERIOD_MS){
                ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, buffer.data(), buffer.size(), timeout_ticks));
            }
            template<std::size_t N>
            void receiveBuffer(uint8_t registerAddr, std::array<uint8_t, N>& buffer, TickType_t timeout_ticks = 3000/portTICK_PERIOD_MS){
                ESP_ERROR_CHECK(i2c_master_transmit_receive(dev_handle, &registerAddr, 1, buffer.data(), buffer.size(), timeout_ticks));
            }
    };
}

#endif  // __I2C_H__
