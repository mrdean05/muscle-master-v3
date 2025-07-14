#include "InternetManager.h"
#include "WiFiManager.h"
#include <string>
#include "Fatfs.h"
#include "InventoryFileManager.h"

namespace APP {

    //std::string inventoryFilePath { std::string(Bsp::Fatfs::basePath) + "/trans" }; 
    
    InternetManager::InternetStatus InternetManager::currentInternetStatus = InternetManager::InternetStatus::WifiDisconnected;
    Bsp::Logger InternetManager::InternetManagerLogger("InternetManager");
    //FileTransferManager InternetManager::inventoryFileManager_(inventoryFilePath);

    void InternetManager::execute(){
        WiFiManager::initialize();

        while (1){
            if (getInternetStatus() == InternetStatus::WifiConnected){
                //InternetManagerLogger.info("Wifi Connected\n");
                //inventoryFileManager_.transferFiles();
                InventoryFileManager::doOperation(InventoryFileManager::Ops::Transfer, nullptr);
                
                vTaskDelay(3000/portTICK_PERIOD_MS);
            } 

            vTaskDelay(2000/portTICK_PERIOD_MS);
        }
    }
}