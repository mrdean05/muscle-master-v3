#include "GpioPin.h"

namespace Bsp{

    std::map<gpio_num_t, GpioPin::Callback> GpioPin::gpioIntrCallbacks;
    bool GpioPin::isServiceInstalled = false;
    

    GpioPin::GpioPin( PinNames ioPin, Direction dir, bool initialLevel,
                        InterruptType intrType, Callback gpioCb ): 
                       ioPinNumber(static_cast<gpio_num_t>(ioPin)), gpio_dir(dir){
        
        
        gpio_config_t io_conf = {};
        io_conf.pin_bit_mask = (1ULL << ioPinNumber);
        io_conf.intr_type = static_cast<gpio_int_type_t>(InterruptType::None);
        io_conf.mode = (dir == Direction::Output) ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT;
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;

        ESP_ERROR_CHECK(gpio_config(&io_conf));

        if (dir == Direction::Output) {
            gpio_set_level(ioPinNumber, initialLevel ? 1 : 0);
        }

        if (intrType != InterruptType::None && gpioCb) {
            registerInterrupt( gpioCb, intrType);
        }
    }


    void GpioPin::setLevel(bool level) {
        gpio_set_level(ioPinNumber, level ? 1 : 0);
    }

    bool GpioPin::getLevel() const {
        return gpio_get_level(ioPinNumber) == 1;
    }

    void GpioPin::toggle() {
        setLevel(!getLevel());
    }

    void IRAM_ATTR GpioPin::isrHandle(void *args){
        gpio_num_t pin = static_cast<gpio_num_t>(reinterpret_cast<uintptr_t>(args));
        auto it = gpioIntrCallbacks.find(pin);
        if (it != gpioIntrCallbacks.end() && it->second){
            it->second();
        }
    }


    void GpioPin::registerInterrupt(Callback cb, InterruptType intrType) {
        if (!isServiceInstalled) {
            gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
            isServiceInstalled = true;
        }

        gpio_isr_handler_add(ioPinNumber, isrHandle, reinterpret_cast<void*>(ioPinNumber));
        gpioIntrCallbacks[ioPinNumber] = cb;
    }

}


