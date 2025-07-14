#include "Register.h"

namespace APP{
    Register::Register(State* parentState_): 
    registerScreenFrame(LCDFrameDisplay::getInstance()){
        parentState = parentState_;
    }


    void Register::entryAction(){

    }

    void Register::existAction(){

    }

    void Register::handleEvent(Events::Event eventType){

    }
}