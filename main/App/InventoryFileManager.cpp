#include "InventoryFileManager.h"
#include <string>
#include "fatfs.h"


namespace APP{

    std::string inventoryPath = std::string(Bsp::Fatfs::basePath);
    //std::string inventoryPath = std::string(Bsp::Fatfs::basePath) + "/trans" + "/file-current.json";
    //std::string inventoryPath = std::string(Bsp::Fatfs::basePath) + "/trans" + "/file-pending.json";
    std::string writefileName = "file-current.json";
    std::string uploadfileName = "file-transfer.json";
 
    FileTransferManager* InventoryFileManager::inventoryFiles_ = nullptr;
    SemaphoreHandle_t InventoryFileManager::inventoryMutex_ = nullptr;
    //bool InventoryFileManager::isInitialized = false;
    
    InventoryFileManager::InventoryFileManager(){

        static FileTransferManager inventoryFiles = FileTransferManager(inventoryPath);
        inventoryFiles_ = &inventoryFiles;
        inventoryMutex_ = xSemaphoreCreateMutex();
        printf("Inventory File Initialized Finvemtory\n");

    }
    
    void InventoryFileManager::doOperation( Ops opsType, char* data ){
        if (opsType == Ops::Write){
            printf("Operation Write-Semaphore\n");
            xSemaphoreTake(inventoryMutex_, portMAX_DELAY);
            printf("Operation Write\n");
            inventoryFiles_->writeToFile(writefileName, data);
            xSemaphoreGive(inventoryMutex_);
        }

        else if (opsType == Ops::Transfer){

            //char fileBuffer[2048];
            
            if (inventoryFiles_->fileExists(uploadfileName)){
                printf("Upload file exist\n");
                if (inventoryFiles_->streamFileToCloud(uploadfileName)){ inventoryFiles_->removeFile(uploadfileName); }
            }
            else {
                if (!inventoryFiles_->fileExists(writefileName)){ printf("File-current doesnt exist\n"); return; }
                xSemaphoreTake(inventoryMutex_, portMAX_DELAY);
                inventoryFiles_->writeToFile(writefileName);
                if(inventoryFiles_->renameFile(writefileName, uploadfileName)){
                    xSemaphoreGive(inventoryMutex_);
                    if (inventoryFiles_->streamFileToCloud(uploadfileName)){ inventoryFiles_->removeFile(uploadfileName); }
                }

            }
            
            /*
            if (inventoryFiles_->fileExists(uploadfileName)){
                if(inventoryFiles_->readFileIntoBuffer(uploadfileName, fileBuffer, sizeof(fileBuffer))){
                    if(inventoryFiles_->transferBuffer(fileBuffer)){ inventoryFiles_->removeFile(uploadfileName); }
                }
            }
            else {
                if (!inventoryFiles_->fileExists(writefileName)){ printf("File-current doesnt exist\n"); return; }
               xSemaphoreTake(inventoryMutex_, portMAX_DELAY);
                if(inventoryFiles_->renameFile(writefileName, uploadfileName)){
                    xSemaphoreGive(inventoryMutex_);
                    if(inventoryFiles_->readFileIntoBuffer(uploadfileName, fileBuffer, sizeof(fileBuffer))){
                        if(inventoryFiles_->transferBuffer(fileBuffer)){ inventoryFiles_->removeFile(uploadfileName); }
                    }
                }
                else {
                    xSemaphoreGive(inventoryMutex_);
                }
            }
            */
        }
    }

    InventoryFileManager* InventoryFileManager::getInstance( void ){
        static InventoryFileManager inventoryFileManager;
        return &inventoryFileManager;
    }


}