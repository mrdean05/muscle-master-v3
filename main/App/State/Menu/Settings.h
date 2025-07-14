#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "ChildState.h"
#include "LCDFrameDisplay.h"

namespace APP{
    class Settings: public ChildState {
        private:
            
            LCDFrameDisplay* settingsScreen;
            Bsp::Logger settingsLogs; 

        public:
            explicit Settings(State* parentState_);
            void entryAction() override;
            void existAction() override;
            void handleEvent(Events::Event eventType) override; 
    };
}

#endif