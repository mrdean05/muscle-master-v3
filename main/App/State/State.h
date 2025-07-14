#ifndef __STATE_H__
#define __STATE_H__
#include <cstdint>

#include "StateTemp.h"
#include "LCDFrameDisplay.h"

namespace APP{

    class StateController; 

    class State : public StateTemp {
        protected:
            StateController* stateControllerContext;
            uint8_t currentChildState; 
    };
}

#endif