#include <cstring>

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#include "Spi.h"

namespace Bsp{
    
    #define SENDER_HOST SPI2_HOST
    SemaphoreHandle_t SPI::semaphore_ = nullptr;
    spi_device_handle_t SPI::handle = nullptr;
    //GpioPin* SPI::spiPinIntr = nullptr;

    Logger SPI::spiLogger("SPI");
    /*
    PinNames SPI::spimosi_ = NO_GPIO;
    PinNames SPI::spimiso_ = NO_GPIO;
    PinNames SPI::spisck_ = NO_GPIO;
    PinNames SPI::spics_ = NO_GPIO;
    PinNames SPI::spiintr_ = NO_GPIO;
    */
    
    SPI::SPI(){ SPI::semaphore_ = xSemaphoreCreateBinary(); }
    void SPI::initialize( PinNames mosi_, PinNames miso_, PinNames sck_, 
                     PinNames cs_, PinNames gpioIntr_){


        spi_bus_config_t buscfg = {};
        buscfg.mosi_io_num = static_cast<int>(mosi_);
        buscfg.miso_io_num = static_cast<int>(miso_);
        buscfg.sclk_io_num = static_cast<int>(sck_);
        buscfg.quadwp_io_num = -1;
        buscfg.quadhd_io_num = -1;
        

        //Configuration for the SPI device on the other side of the bus
        spi_device_interface_config_t devcfg = {};
        devcfg.command_bits = 0;
        devcfg.address_bits = 0;
        devcfg.dummy_bits = 0;
        devcfg.mode = 0;
        devcfg.duty_cycle_pos = 128;      //50% duty cycle
        devcfg.cs_ena_posttrans = 3;
        devcfg.clock_speed_hz = 1000000;
        devcfg.spics_io_num = static_cast<int>(cs_);
        //Keep the CS low 3 cycles after transaction, to stop slave from missing the last bit when CS has less propagation delay than CLK
        devcfg.queue_size = 3;
        
        
        //ToDo: Change this to use the Gpio Class
        //static GpioPin spiPinIntr_ = GpioPin(gpioIntr_, Bsp::GpioPin::Direction::Input,
        //                                    false, Bsp::GpioPin::InterruptType::RisingEdge, []());
        //spiPinIntr = 
        
        //GPIO config for the handshake line.
        gpio_config_t gpio_conf = {
            .pin_bit_mask = BIT64(static_cast<int>(gpioIntr_)),
            .mode =  GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_ENABLE,
            .intr_type = GPIO_INTR_POSEDGE,
        };

        //Set up handshake line interrupt.
        ESP_ERROR_CHECK(gpio_config(&gpio_conf));
        //ESP_ERROR_CHECK(gpio_install_isr_service(0));
        ESP_ERROR_CHECK(gpio_set_intr_type(static_cast<gpio_num_t>(gpioIntr_), GPIO_INTR_POSEDGE));
        ESP_ERROR_CHECK(gpio_isr_handler_add(static_cast<gpio_num_t>(gpioIntr_), gpioIsrHandler, NULL));

        ESP_ERROR_CHECK(spi_bus_initialize(SENDER_HOST, &buscfg, SPI_DMA_CH_AUTO));
        ESP_ERROR_CHECK(spi_bus_add_device(SENDER_HOST, &devcfg, &handle));

        SPI::spiLogger.info("SPI Drivers initialization successful\n");

    }


    void SPI::clearIntrFlag (void){
        spiMessagePayload clearFlag = {};
        clearFlag.command = command_type::CLEAR_GPIO;
        
        spi_transaction_t clearGpioTrans;
        memset(&clearGpioTrans, 0, sizeof(spi_transaction_t));
        clearGpioTrans.length = sizeof(clearFlag) * 8;
        clearGpioTrans.tx_buffer = &clearFlag;
        clearGpioTrans.rx_buffer = NULL;

        // Send command to slave
        ESP_ERROR_CHECK(spi_device_transmit(handle, &clearGpioTrans));
    }

    void IRAM_ATTR SPI::gpioIsrHandler(void* arg){
        //Sometimes due to interference or ringing or something, we get two irqs after eachother. This is solved by
        //looking at the time between interrupts and refusing any interrupt too close to another one.
        static uint32_t lasthandshaketime_us;
        uint32_t currtime_us = esp_timer_get_time();
        uint32_t diff = currtime_us - lasthandshaketime_us;
        if (diff < 1000) {
            return; //ignore everything <1ms after an earlier irq
        }
        lasthandshaketime_us = currtime_us;

        //Give the semaphore.
        BaseType_t must_yield = pdFALSE;
        // Signal task_done_sem when the slave completes a task
        if (xSemaphoreGiveFromISR(semaphore_, &must_yield) != pdFALSE) {
            portYIELD_FROM_ISR();
        }
    }

    void SPI::waitForScreen(void){
        if (xSemaphoreTake(semaphore_, portMAX_DELAY)) {
            vTaskDelay(30/portTICK_PERIOD_MS);
            clearIntrFlag();
        }
    }

    void SPI::sendSpiData( spiMessagePayload& command ){
        spi_transaction_t t;
        memset(&t, 0, sizeof(spi_transaction_t));
        t.length = sizeof(command) * 8;
        t.tx_buffer = &command;
        t.rx_buffer = NULL;
        // Send command to slave
        spi_device_transmit(handle, &t);

        // Wait for the handshake signal from the slave
        if (xSemaphoreTake(semaphore_, portMAX_DELAY)) {
            // Handshake received, proceed
            vTaskDelay(30/portTICK_PERIOD_MS);
            clearIntrFlag();
        }

    }

}