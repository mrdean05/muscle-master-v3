#include "InternetManager.h"
#include "WiFiManager.h"
#include <string>
#include "Fatfs.h"
#include "SalesFileManager.h"

namespace APP {
    
    InternetManager::InternetStatus InternetManager::currentInternetStatus = InternetManager::InternetStatus::WifiDisconnected;
    Bsp::Logger InternetManager::InternetManagerLogger("InternetManager");

    void InternetManager::execute(){
        WiFiManager::initialize();

        while (1){
            if (getInternetStatus() == InternetStatus::WifiConnected){
                //InternetManagerLogger.info("Wifi Connected\n");
                //SalesFileManager_.transferFiles();
                SalesFileManager::doOperation(SalesFileManager::Ops::Transfer, nullptr);
                
                vTaskDelay(3000/portTICK_PERIOD_MS);
            } 

            vTaskDelay(2000/portTICK_PERIOD_MS);
        }
    }
}