#ifndef __INVENTORY_FILE_MANAGER_H__
#define __INVENTORY_FILE_MANAGER_H__

#include "FileTransferManager.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace APP{
    class InventoryFileManager{
        private:
            static FileTransferManager* inventoryFiles_;
            static SemaphoreHandle_t inventoryMutex_;
            InventoryFileManager();

        public:
            enum Ops{
                Write,
                Read,
                Transfer
            };
            
            static void doOperation(Ops opsType, char* data);
            static InventoryFileManager* getInstance( void );
    };
}



#endif