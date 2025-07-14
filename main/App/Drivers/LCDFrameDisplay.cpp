#include "LCDFrameDisplay.h"
#include "StateEnums.h"

#include <cstring>

namespace APP {

    Bsp::SPI* LCDFrameDisplay::spiDriver_ = nullptr;
    SemaphoreHandle_t LCDFrameDisplay::mutex_ = nullptr;
    Bsp::Logger LCDFrameDisplay::lcdLogger("LCD FRAME DISPLAY");
    

    LCDFrameDisplay::LCDFrameDisplay(){

        mutex_ = xSemaphoreCreateMutex();
        static Bsp::SPI spiDriver;
        spiDriver_  = &spiDriver;
    }

    LCDFrameDisplay* LCDFrameDisplay::getInstance(){
        static LCDFrameDisplay frameDisplay;
        return &frameDisplay;
    }

    void LCDFrameDisplay::screenDisplayLock(){
        xSemaphoreTake(mutex_, portMAX_DELAY);
    }

    void LCDFrameDisplay::screenDisplayUnlock(){
        xSemaphoreGive(mutex_);
    }

    void LCDFrameDisplay::initializeLCD(){
        lcdLogger.info("Initializing LCD Screen\n");
        spiDriver_->initialize(Bsp::PinNames::ESP_MOSI, Bsp::PinNames::ESP_MISO, 
                    Bsp::PinNames::ESP_SCK, Bsp::PinNames::ESP_CS, Bsp::PinNames::ESP_INT1);
        spiDriver_->waitForScreen();
        lcdLogger.info("Done initialization\n");
    }

    void LCDFrameDisplay::launchScreen(){
        Bsp::spiMessagePayload screen_command = {};
        screen_command.command = Bsp::command_type::STATE;
        screen_command.data.state_type = Bsp::statetype_t::LAUNCH_STATE;
    
        screenDisplayLock();
        Bsp::SPI::sendSpiData(screen_command);
        screenDisplayUnlock();
    
    }

    void LCDFrameDisplay::launchMenu( void ){
        Bsp::spiMessagePayload screen_command = {};
        screen_command.command = Bsp::command_type::STATE;
        screen_command.data.state_type = Bsp::statetype_t::MENU_STATE;

        screenDisplayLock();
        Bsp::SPI::sendSpiData(screen_command);
        screenDisplayUnlock();
    }

    void LCDFrameDisplay::launchInventoryTable( void ){
        Bsp::spiMessagePayload screen_command = {};
        screen_command.command = Bsp::command_type::CHILDSTATE;
        screen_command.data.childstate = Bsp::childState_type::MENU_INVENTORY;

        screenDisplayLock();
        Bsp::SPI::sendSpiData(screen_command);
        screenDisplayUnlock();
    }

    void LCDFrameDisplay::productDisplay(const Bsp::product_obj_t& item_objt){
        Bsp::spiMessagePayload screen_command = {};
        screen_command.command = Bsp::command_type::EVENT;
        screen_command.data.event.event_type = Bsp::EventType::EVT_BARCODE_SCAN;
        screen_command.data.event.event_data.product_data = item_objt;

        screenDisplayLock();
        Bsp::SPI::sendSpiData(screen_command);
        screenDisplayUnlock();
    }

    void LCDFrameDisplay::productDescriptionTextArea(const char key){
        Bsp::spiMessagePayload screen_command = {};
        screen_command.command = Bsp::command_type::EVENT;
        screen_command.data.event.event_type = Bsp::EventType::EVT_USER_INPUT;
        screen_command.data.event.event_data.key_pressed = key;

        screenDisplayLock();
        Bsp::SPI::sendSpiData(screen_command);
        screenDisplayUnlock();
    }

    void LCDFrameDisplay::productDescriptionEnterTextArea(const Bsp::product_obj_t& item_objt){
        Bsp::spiMessagePayload screen_command = {};
        screen_command.command = Bsp::command_type::EVENT;
        screen_command.data.event.event_type = Bsp::EventType::EVT_ENTER_PRESSED;
        screen_command.data.event.event_data.product_data = item_objt;

        screenDisplayLock();
        Bsp::SPI::sendSpiData(screen_command);
        screenDisplayUnlock();
    }

    void LCDFrameDisplay::displayTotalPrice(std::string totalPrice){
        Bsp::spiMessagePayload screen_command = {};
        screen_command.command = Bsp::command_type::EVENT;
        screen_command.data.event.event_type = Bsp::EventType::EVT_ENTER_PRESSED;
        strcpy(screen_command.data.event.event_data.text, totalPrice.c_str());

        screenDisplayLock();
        Bsp::SPI::sendSpiData(screen_command);
        screenDisplayUnlock();
    }

    void LCDFrameDisplay::sendInput(const char key){
        Bsp::spiMessagePayload screen_command = {};
        screen_command.command = Bsp::command_type::EVENT;
        screen_command.data.event.event_type = Bsp::EventType::EVT_USER_INPUT;
        screen_command.data.event.event_data.key_pressed = key;

        screenDisplayLock();
        Bsp::SPI::sendSpiData(screen_command);
        screenDisplayUnlock();
    }

    void LCDFrameDisplay::returnTable(){

        Bsp::spiMessagePayload screen_command = {};
        screen_command.command = Bsp::command_type::EVENT;
        screen_command.data.event.event_type = Bsp::EventType::EVT_ENTER_PRESSED;
        screen_command.data.event.event_data.key_pressed = '=';

        screenDisplayLock();
        Bsp::SPI::sendSpiData(screen_command);
        screenDisplayUnlock();        

    }

}