#ifndef __REGISTER_H__
#define __REGISTER_H__
#include "ChildState.h"
#include "Events.h"

namespace APP{
    class Register: public ChildState {
        private:
            LCDFrameDisplay* registerScreenFrame;
        public:
            explicit Register(State* parentState_);
            
            void entryAction() override;
            void existAction() override;
            void handleEvent(Events::Event eventType) override; 

    };
}


#endif