#include "Startup.h"
#include "Launch.h"
#include "Register.h"


namespace APP {

    std::array<ChildState*, 2> Startup::childStates = { nullptr, nullptr };
    Startup::Startup(StateController* context, ChildState_ childState){
        stateControllerContext = context;
        currentChildState = static_cast<uint8_t>(childState); 

        static Launch launchChildState = Launch(this);
        static Register registerChildState = Register(this);
        childStates[0] = &launchChildState;
        childStates[1] = &registerChildState;

        //entryAction();
    }
    
    void Startup::entryAction() {
        currentChildState = static_cast<uint8_t>(ChildState_::Launch);
        childStates[currentChildState - 1]->entryAction();
    }

    void Startup::handleEvent(Events::Event eventType){
        //if (currentChildState == static_cast<uint8_t>(ChildState_::Null)) { return; }
        //childStates[currentChildState - 1]->handleEvent(eventType);
        //existAction();
    }

    void Startup::existAction(){
        currentChildState = static_cast<uint8_t>(ChildState_::Null);
    }




}