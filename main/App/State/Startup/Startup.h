#ifndef __STARTUP_H__
#define __STARTUP_H__
#include "State.h"
#include "ChildState.h"
#include "Events.h"
#include <array>

namespace APP {
    class Startup: public State{
        private:
            static std::array <ChildState*, 2> childStates;

        public: 
            enum class ChildState_: uint8_t {
                Null,
                Launch,
                Register
            };
            explicit Startup(StateController* context, ChildState_ childState = ChildState_::Null);
            
            void entryAction() override;
            void existAction() override;
            void handleEvent(Events::Event eventType) override; 
    };
}
#endif