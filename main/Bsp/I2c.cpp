#include "I2c.h"
#include "PinNames.h"

namespace Bsp{
                 
    #define I2C_MASTER_NUM              0
    #define I2C_MASTER_FREQ_HZ          400000 
    
    bool I2c::isBusInitialized = false;
    SemaphoreHandle_t I2c::mutex_ = nullptr;
    i2c_master_bus_handle_t I2c::bus_handle;

    I2c::I2c(uint16_t deviceAddr):deviceAddr_(deviceAddr){
        if (!isBusInitialized){
            initializeBus();
            mutex_ = xSemaphoreCreateMutex();
            isBusInitialized = true;
        }

        i2c_device_config_t dev_config = {};
        dev_config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
        dev_config.device_address = deviceAddr_;
        dev_config.scl_speed_hz = I2C_MASTER_FREQ_HZ;
        
        ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_config, &dev_handle));
    }

    void I2c::initializeBus(void){
        i2c_master_bus_config_t bus_config = {};
        bus_config.i2c_port = I2C_MASTER_NUM;
        bus_config.sda_io_num = static_cast<gpio_num_t>(PinNames::SDA);
        bus_config.scl_io_num = static_cast<gpio_num_t>(PinNames::SCL);
        bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
        bus_config.glitch_ignore_cnt = 7;
        bus_config.flags.enable_internal_pullup = false;
        
        ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));
    }
}