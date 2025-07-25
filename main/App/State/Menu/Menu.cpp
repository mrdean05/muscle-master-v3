#include "Menu.h"
#include "Sales/Sales.h"
#include "Calculator.h"
#include "Settings.h"
#include "KeypadCharacter.h"

namespace APP {

    Bsp::Logger Menu::menuLogger("Menu");
    std::array<ChildState*, MENUCHILDSTATE> Menu::states = { nullptr, nullptr, nullptr };
    Menu::Menu(StateController* context, MenuChildState menuChildState):
    menuScreenFrame(LCDFrameDisplay::getInstance()) {
        stateControllerContext = context;
        currentChildState = static_cast<uint8_t>(menuChildState);

        static Sales salesState = Sales(this);
        static Calculator calculatorState = Calculator(this);
        static Settings settingsState = Settings(this);

        states[0] = &salesState;
        states[1] = &calculatorState;
        states[2] = &settingsState;

    }
    
    void Menu::entryAction(){
        currentChildState = static_cast<uint8_t>(MenuChildState::Null);
        menuLogger.info("Display Menu Buffer\n");
        menuScreenFrame->launchMenu();
    }

    void Menu::existAction(){
        states[currentChildState - 1]->existAction();
        currentChildState = static_cast<uint8_t>(MenuChildState::Null);
    }

    void Menu::handleEvent(Events::Event eventType){
        if (currentChildState == static_cast<uint8_t>(MenuChildState::Null)){
            if (eventType == Events::Event::AlphaNumeric){
                switch(KeypadCharacter::getPressedKey2()){
                    case '1':
                        menuLogger.info("Inventory Selected\n");
                        break;
                    case '2':
                        menuLogger.info("Sales Selected\n");
                        currentChildState = static_cast<uint8_t>(MenuChildState::Sales);
                        states[currentChildState - 1]->entryAction();

                        break;
                    case '3':
                        menuLogger.info("Calculator Selected\n");
                        currentChildState = static_cast<uint8_t>(MenuChildState::Calculator);
                        states[currentChildState - 1]->entryAction();
                        break;
                    default:
                        menuLogger.info("Select the right thing\n");
                        break;
                }
            }
        }
        else {
            states[currentChildState - 1]->handleEvent(eventType);
        }
    }


}