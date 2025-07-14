#ifndef __TIMER_H__
#define __TIMER_H__

#include "driver/gptimer.h"

namespace Bsp {

    class Timer {
        public:
            Timer();
            void setAlarmCount( uint8_t seconds );
            void setEventCalbk( gptimer_alarm_cb_t evtClk );
            void enableTimer( void );
            void startTimer( void );
            void stopTimer( void );
            void disableTimer( void );

        private:
            bool isTimerOn = false;
            gptimer_handle_t gpTimer;
            gptimer_config_t gpTimerConfig;
            gptimer_event_callbacks_t evtCalback;
            gptimer_alarm_config_t alarmConfig;

    };

}

#endif