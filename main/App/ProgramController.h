#ifndef __PROGRAMCONTROLER_H__
#define __PROGRAMCONTROLER_H__

#include <string>

#include "StateController.h"
#include "KeypadCharacter.h"
#include "LCDFrameDisplay.h"
#include "PowerManager.h"
#include "Logger.h"
#include "StateController.h"
#include "Fatfs.h"
#include "RtcModule.h"
#include "Pmic.h"
#include "BatHealth.h"
#include "Timer.h"
#include "SalesFileManager.h"

namespace APP{

    class ProgramController {
        public:
            ProgramController();
            static void config();
            static void execute();
            static void appFlow(void *args);
            static void internetFlow(void *args);
            static std::string getBarcodeData(void);
            static bool alarmEvtCalback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);

        private:
            static inline bool checkBattery = true;
            static std::string barcodeData;
            static inline QueueHandle_t barcode_queue;
            static LCDFrameDisplay* screenFrame;
            static StateController* stateControl_;
            static KeypadCharacter* keyPress;
            static PowerManager* powerManager;
            static Bsp::Fatfs* fileSystem;
            static RtcModule* rtcClock;
            static Bsp::Logger logger;
            static Pmic* pmicPowerManager_;
            static BatHealth* batteryManager_;
            static Bsp::Timer* batTimer_;

    }; 


}


#endif