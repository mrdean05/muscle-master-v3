#ifndef __STATECONTROLLER_H__
#define __STATECONTROLLER_H__
#include "KeypadCharacter.h"
#include "Logger.h"
#include "State.h"
#include "Menu.h"
#include "Startup.h"
#include "Events.h"

namespace APP {
    class StateController {

        public:
            enum class DeviceState : uint8_t{
                Null,
                Startup,
                Menu,
                Shutdown
            };

            StateController();
            static void handleEvent(Events::Event eventType);
            static DeviceState getCurrentState();

        private:

 
            static Bsp::Logger stateControllerLogger;
            static DeviceState currentState;
            static void handleStateEvent();
            static std::array <State*, 2> states; 
    };
}



#endif