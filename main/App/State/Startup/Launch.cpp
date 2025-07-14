#include "Launch.h"
#include "NvsFlashManager.h"

namespace APP {

    using checkRegistrationStatus =  Bsp::NvsFlashManager;

    Bsp::Logger Launch::launchLogger("Launch");
    Launch::Launch(State* parentState_):
    LaunchScreenFrame(LCDFrameDisplay::getInstance())
    {
        parentState = parentState_;
    }

    void Launch::entryAction(){
        //ToDo Load the screen
        launchLogger.info("Launching Muscle Logo\n");
        LaunchScreenFrame->launchScreen();
    }
    void Launch::existAction(){
        //ToDo Check if user has registered
        checkRegistrationStatus::open();
        

    }
    void Launch::handleEvent(Events::Event eventType){
        //entryAction();
        //existAction();
    } 
}