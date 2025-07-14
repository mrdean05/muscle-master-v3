#include "Calculator.h"

namespace APP{
    Calculator::Calculator( State* parentState_ ):
    calculatorScreen(LCDFrameDisplay::getInstance()),
    calculatorLogs("Calculator")
    {
        parentState = parentState_;
    }

    void Calculator::entryAction(){

    }

    void Calculator::existAction(){

    }

    void Calculator::handleEvent(Events::Event eventType){

    }

}