#include "Settings.h"

namespace APP{
    Settings::Settings( State* parentState_ ):
    settingsScreen(LCDFrameDisplay::getInstance()),
    settingsLogs("Settings")
    {
        parentState = parentState_;
    }

    void Settings::entryAction(){

    }

    void Settings::existAction(){

    }

    void Settings::handleEvent(Events::Event eventType){

    }

}