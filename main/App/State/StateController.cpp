#include "StateController.h"



namespace APP {

    
    Bsp::Logger StateController::stateControllerLogger("StateController");
    StateController::DeviceState StateController::currentState = StateController::DeviceState::Null;
    std::array <State*, 2> StateController::states = { nullptr, nullptr };

    StateController::StateController(){

        static Startup startupState = Startup(this);
        static Menu menuState = Menu(this);
        states[0] = &startupState;
        states[1] = &menuState;
    }

    void StateController::handleEvent(Events::Event eventType){

        switch (eventType){
            case Events::Event::Launch:
                stateControllerLogger.info("Startup State\n");
                currentState = DeviceState::Startup;
                states[static_cast<int>(currentState) - 1]->entryAction(); // ToDo here should be entryAction
                break;
            case Events::Event::Menu:
                states[static_cast<int>(currentState) - 1]->existAction();
                stateControllerLogger.info("Menu State\n");
                currentState = DeviceState::Menu;
                states[static_cast<int>(currentState) - 1]->entryAction();
                break;
            //case Events::Event::AlphaNumeric:
                //break;
            default:
                states[static_cast<int>(currentState) - 1]->handleEvent(eventType);
                break;
        }


    }

}