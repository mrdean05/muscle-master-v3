#ifndef __LAUNCH_H__
#define __LAUNCH_H__
#include "ChildState.h"
#include "Events.h"

namespace APP {
    class Launch: public ChildState {
        private:
            LCDFrameDisplay* LaunchScreenFrame;
            static Bsp::Logger launchLogger;
        public: 

            explicit Launch(State* parentState_);
            void entryAction() override;
            void existAction() override;
            void handleEvent(Events::Event eventType) override; 

    };
}

#endif