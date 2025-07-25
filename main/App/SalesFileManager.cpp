#include "SalesFileManager.h"
#include <string>
#include "fatfs.h"


namespace APP{

    std::string salesPath = std::string(Bsp::Fatfs::basePath);
    //std::string salesPath = std::string(Bsp::Fatfs::basePath) + "/trans" + "/file-current.json";
    //std::string salesPath = std::string(Bsp::Fatfs::basePath) + "/trans" + "/file-pending.json";
    std::string writefileName = "file-current.json";
    std::string uploadfileName = "file-transfer.json";
 
    FileTransferManager* SalesFileManager::salesFiles_ = nullptr;
    SemaphoreHandle_t SalesFileManager::salesMutex_ = nullptr;
    //bool SalesFileManager::isInitialized = false;
    
    SalesFileManager::SalesFileManager(){

        static FileTransferManager salesFiles = FileTransferManager(salesPath);
        salesFiles_ = &salesFiles;
        salesMutex_ = xSemaphoreCreateMutex();
        printf("Inventory File Initialized Finvemtory\n");

    }
    
    void SalesFileManager::doOperation( Ops opsType, char* data ){
        if (opsType == Ops::Write){
            printf("Operation Write-Semaphore\n");
            xSemaphoreTake(salesMutex_, portMAX_DELAY);
            printf("Operation Write\n");
            salesFiles_->writeToFile(writefileName, data);
            xSemaphoreGive(salesMutex_);
        }

        else if (opsType == Ops::Transfer){

            //char fileBuffer[2048];
            
            if (salesFiles_->fileExists(uploadfileName)){
                printf("Upload file exist\n");
                if (salesFiles_->streamFileToCloud(uploadfileName)){ salesFiles_->removeFile(uploadfileName); }
            }
            else {
                if (!salesFiles_->fileExists(writefileName)){ printf("File-current doesnt exist\n"); return; }
                xSemaphoreTake(salesMutex_, portMAX_DELAY);
                salesFiles_->writeToFile(writefileName);
                if(salesFiles_->renameFile(writefileName, uploadfileName)){
                    xSemaphoreGive(salesMutex_);
                    if (salesFiles_->streamFileToCloud(uploadfileName)){ salesFiles_->removeFile(uploadfileName); }
                }

            }
            
            /*
            if (salesFiles_->fileExists(uploadfileName)){
                if(salesFiles_->readFileIntoBuffer(uploadfileName, fileBuffer, sizeof(fileBuffer))){
                    if(salesFiles_->transferBuffer(fileBuffer)){ salesFiles_->removeFile(uploadfileName); }
                }
            }
            else {
                if (!salesFiles_->fileExists(writefileName)){ printf("File-current doesnt exist\n"); return; }
               xSemaphoreTake(salesMutex_, portMAX_DELAY);
                if(salesFiles_->renameFile(writefileName, uploadfileName)){
                    xSemaphoreGive(salesMutex_);
                    if(salesFiles_->readFileIntoBuffer(uploadfileName, fileBuffer, sizeof(fileBuffer))){
                        if(salesFiles_->transferBuffer(fileBuffer)){ salesFiles_->removeFile(uploadfileName); }
                    }
                }
                else {
                    xSemaphoreGive(salesMutex_);
                }
            }
            */
        }
    }

    SalesFileManager* SalesFileManager::getInstance( void ){
        static SalesFileManager salesFileManager;
        return &salesFileManager;
    }


}