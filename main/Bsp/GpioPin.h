#ifndef __GPIO_PIN_H__
#define __GPIO_PIN_H__

#include "driver/gpio.h"
#include "esp_attr.h"

#include "PinNames.h"
#include <functional>
#include <map>


namespace Bsp{
    class GpioPin {
        public: 
            enum class Direction : uint8_t{
                Input,
                Output
            };

            enum class InterruptType {
                None = GPIO_INTR_DISABLE,
                RisingEdge = GPIO_INTR_POSEDGE,
                FallingEdge = GPIO_INTR_NEGEDGE,
                AnyEdge = GPIO_INTR_ANYEDGE,
                LowLevel = GPIO_INTR_LOW_LEVEL,
                HighLevel = GPIO_INTR_HIGH_LEVEL
            };

            using Callback = std::function<void()>;

            GpioPin( PinNames ioPin, Direction dir, bool initialLevel = false,
                InterruptType intrType = InterruptType::None, Callback gpioCb = nullptr );

            //GpioPin( PinNames ioPin, Direction dir, bool initialLevel = false);
            void setLevel (bool level);
            bool getLevel () const;
            void toggle ();

        private:
            gpio_num_t ioPinNumber;
            Direction gpio_dir;

            static void IRAM_ATTR isrHandle(void *args);
            void registerInterrupt(Callback cb, InterruptType isrType);
            static std::map<gpio_num_t, Callback> gpioIntrCallbacks;
            static bool isServiceInstalled;

    };
}


#endif