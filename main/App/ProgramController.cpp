
#include "ProgramController.h"
#include "Events.h"
#include "usb_driver.h"
#include "InternetManager.h"
#include "NvsFlashManager.h"
#include "Board.h"


namespace APP {

    static constexpr const char* TAG = "ProgramController";

    LCDFrameDisplay* ProgramController::screenFrame = nullptr;
    PowerManager* ProgramController::powerManager = nullptr;
    Bsp::Logger ProgramController::logger = Bsp::Logger(TAG);
    StateController* ProgramController::stateControl_ = nullptr;
    KeypadCharacter* ProgramController::keyPress = nullptr;
    Bsp::Fatfs* ProgramController::fileSystem = nullptr;
    RtcModule* ProgramController::rtcClock = nullptr;
    Pmic* ProgramController::pmicPowerManager_ = nullptr;
    BatHealth* ProgramController::batteryManager_ = nullptr;
    Bsp::Timer* ProgramController::batTimer_ = nullptr;

    std::string ProgramController::barcodeData(14, '\0');

    ProgramController::ProgramController(){

        static PowerManager powerManager_;
        powerManager = &powerManager_;
        
        static LCDFrameDisplay* screenFrame_ = LCDFrameDisplay::getInstance();
        screenFrame = screenFrame_;

        static KeypadCharacter* keyPress_ = KeypadCharacter::getInstance();
        keyPress = keyPress_;

        static Bsp::Fatfs* fileSystem_ = Bsp::Fatfs::getInstance();
        fileSystem = fileSystem_;

        static StateController stateControl;
        stateControl_ = &stateControl;

        static RtcModule rtcClock_;
        rtcClock = &rtcClock_;

        static Pmic pmicPowerManager;
        pmicPowerManager_ = &pmicPowerManager;

        static BatHealth batteryManager;
        batteryManager_ = &batteryManager;

        static Bsp::Timer batTimer;
        batTimer_ = &batTimer;

        (void)SalesFileManager::getInstance();

        Bsp::NvsFlashManager();

        Bsp::Board();

    }

    void ProgramController::config(){

        powerManager->setPowerOn();
        screenFrame->initializeLCD();

        printf("Power On Configuration: %x\n", static_cast<uint16_t>(pmicPowerManager_->getPowerOnConfig()));
        printf("System Status Config: %x\n", static_cast<uint16_t>(pmicPowerManager_->getSystemStatus()));
        printf("Fault Status Config: %x\n", static_cast<uint16_t>(pmicPowerManager_->getFaultStatus()));
    
        powerManager->setPowerOn();
        printf("Battery Voltage Level: %f\n", batteryManager_->getBatteryVoltageLevel());
        printf("SoC Level: %d\n", batteryManager_->getBatterySoc());
        
        batTimer_->setAlarmCount(2);
        batTimer_->setEventCalbk(alarmEvtCalback);
        batTimer_->enableTimer();
        batTimer_->startTimer();
        powerManager->setPowerOn();

    }

    void ProgramController::execute(){

        powerManager->setPowerOn();
        barcode_queue = xQueueCreate(2, 14 * sizeof(char));

        xTaskCreatePinnedToCore(appFlow, "App flow task", 10  * 1024, static_cast<void*>(&barcode_queue), 1, NULL, 1);
        xTaskCreatePinnedToCore(USBDriver_initTask, "USB task", 8 * 1024, static_cast<void*>(&barcode_queue), 1, NULL, 1);
        xTaskCreatePinnedToCore(internetFlow, "InternetManager", 8 * 1024, static_cast<void*>(&barcode_queue), 1, NULL, 0);

    }

    void ProgramController::appFlow(void *args){
        StateController::handleEvent(Events::Event::Launch);
        StateController::handleEvent(Events::Event::Menu);

        QueueHandle_t barcode_queue = *(static_cast<QueueHandle_t*>(args));
        powerManager->setPowerOn();

        while (1){

            char charPressed =  keyPress->getPressedKey();
            if (charPressed != '\0'){
                Events::Event eventType = Events::getEventFromChar(charPressed);
                StateController::handleEvent(eventType);
            }

            if (checkBattery) {
                printf("Battery Voltage Level: %f\n", batteryManager_->getBatteryVoltageLevel());
                printf("SoC Level: %d\n", batteryManager_->getBatterySoc());
                checkBattery = false;
            }

            //ToDo find a way to active this only when barocde is to be used
            if(xQueueReceive(barcode_queue, barcodeData.data(), 10/portTICK_PERIOD_MS) == pdTRUE){
                StateController::handleEvent(Events::Event::Barcode);
            }

            if (!powerManager->getPushButtonOutState()) {
                logger.info("PowerOff\n");
                vTaskDelay(200/portTICK_PERIOD_MS);
                powerManager->setPowerOff(); 
            }

        }
    }

    void ProgramController::internetFlow(void* args){
        InternetManager::execute();
    }

    std::string ProgramController::getBarcodeData(){
        return barcodeData;
    }

    bool ProgramController::alarmEvtCalback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx){
        checkBattery = true;
        return true;
    }

}