#ifndef __PIN_NAMES_H__
#define __PIN_NAMES_H__

#include "driver/gpio.h"

#include <cstdint>

namespace Bsp{
    enum class PinNames {
        NO_GPIO         =   GPIO_NUM_NC,
        ESP_INT1        =   GPIO_NUM_12,
        ESP_INT2        =   GPIO_NUM_3,
        MEM_SCK         =   GPIO_NUM_5,
        MEM_MOSI        =   GPIO_NUM_6,
        MEM_MISO        =   GPIO_NUM_8,
        MEM_CS         =   GPIO_NUM_7,
        ESP_MISO        =   GPIO_NUM_9,
        ESP_MOSI        =   GPIO_NUM_10,
        ESP_CS          =   GPIO_NUM_11,
        ESP_SCK         =   GPIO_NUM_46,
        BAT_ALERT_INT   =   GPIO_NUM_13,
        PCF8523_INT     =   GPIO_NUM_14,
        KEY_INT         =   GPIO_NUM_4,
        STM6600_INT      =   GPIO_NUM_21,
        BAT_CHARGER_INT =   GPIO_NUM_47,
        SCL             =   GPIO_NUM_16,
        SDA             =   GPIO_NUM_17,
        KEY_RESET       =   GPIO_NUM_18,
        PS_HOLD         =   GPIO_NUM_38,
        PB_OUT          =   GPIO_NUM_45,
        BAT_CHARGER_EN  =   GPIO_NUM_48
    };
}


#endif