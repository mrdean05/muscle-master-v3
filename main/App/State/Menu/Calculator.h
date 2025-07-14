#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include "ChildState.h"
#include "LCDFrameDisplay.h"

namespace APP{
    class Calculator: public ChildState {
        private:
            
            LCDFrameDisplay* calculatorScreen;
            Bsp::Logger calculatorLogs; 

        public:
            explicit Calculator(State* parentState_);
            void entryAction() override;
            void existAction() override;
            void handleEvent(Events::Event eventType) override; 
    };
}

#endif