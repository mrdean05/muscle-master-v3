#ifndef __SALES_FILE_MANAGER_H__
#define __SALES_FILE_MANAGER_H__

#include "FileTransferManager.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace APP{
    class SalesFileManager{
        private:
            static FileTransferManager* salesFiles_;
            static SemaphoreHandle_t salesMutex_;
            SalesFileManager();

        public:
            enum Ops{
                Write,
                Read,
                Transfer
            };
            
            static void doOperation(Ops opsType, char* data);
            static SalesFileManager* getInstance( void );
    };
}



#endif