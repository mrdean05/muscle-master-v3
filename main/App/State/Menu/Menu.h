#ifndef __MENU_H__
#define __MENU_H__
#include <array>

#include "State.h"
#include "ChildState.h"

namespace APP {
    #define MENUCHILDSTATE 3

    class Menu: public State{
        private:
            LCDFrameDisplay* menuScreenFrame;
            static Bsp::Logger menuLogger;
            static std::array<ChildState*, MENUCHILDSTATE> states;
        
        public:
            enum class MenuChildState: uint8_t {
                Null,
                Sales,
                Calculator,
                Settings
            };
            explicit Menu(StateController* context, MenuChildState childState = MenuChildState::Null);
           
            void entryAction() override;
            void existAction() override;
            void handleEvent(Events::Event eventType) override; 
    };
}
#endif