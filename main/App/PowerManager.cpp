#include "PowerManager.h"

namespace APP{
    PowerManager::PowerManager():
    psHold(Bsp::PinNames::PS_HOLD, Bsp::GpioPin::Direction::Output),
    pbOut(Bsp::PinNames::PB_OUT, Bsp::GpioPin::Direction::Input),
    stm6600Int(Bsp::PinNames::STM6600_INT, Bsp::GpioPin::Direction::Input, false, Bsp::GpioPin::InterruptType::FallingEdge, std::bind(&PowerManager::triggerPowerOffCalback, this))
    {}

    void PowerManager::setPowerOn( void ){
        psHold.setLevel(true);
    }

    void PowerManager::setPowerOff( void ){
        psHold.setLevel(false);
    }

    bool PowerManager::getPushButtonOutState( void ){
        return pbOut.getLevel();
    }

    bool PowerManager::getTriggerOffState( void ){
        return triggerOffState;
    }

    void PowerManager::triggerPowerOffCalback( void ){
        triggerOffState = true;
    }


}