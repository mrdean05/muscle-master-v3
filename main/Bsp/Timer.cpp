#include "Timer.h"

namespace Bsp {
    Timer::Timer(){
        gpTimerConfig.clk_src       =   GPTIMER_CLK_SRC_DEFAULT;
        gpTimerConfig.direction     =   GPTIMER_COUNT_UP;
        gpTimerConfig.resolution_hz =   1 * 1000 * 1000;
        gpTimerConfig.intr_priority =   0;

        gptimer_new_timer(&gpTimerConfig, &gpTimer);
    }

    void Timer::setAlarmCount( uint8_t seconds ){
        alarmConfig.reload_count = 0;
        alarmConfig.alarm_count = seconds * 1000 * 1000;
        alarmConfig.flags.auto_reload_on_alarm = true;
        gptimer_set_alarm_action(gpTimer, &alarmConfig);
    }

    void Timer::setEventCalbk( gptimer_alarm_cb_t evtClk ){
        evtCalback.on_alarm = evtClk;
        gptimer_register_event_callbacks(gpTimer, &evtCalback, NULL);
    }

    void Timer::enableTimer( void ){
        gptimer_enable(gpTimer);
    }

    void Timer::startTimer( void ){
        if ( isTimerOn == false ){
            gptimer_start(gpTimer);
            isTimerOn = true;
        }
    }

    void Timer::stopTimer( void ){
        if ( isTimerOn == true ){
            gptimer_stop(gpTimer);
            isTimerOn = false;
        }
    }
    
    void Timer::disableTimer( void ){
        stopTimer();
        gptimer_disable(gpTimer);
        gptimer_del_timer(gpTimer);
    }


}