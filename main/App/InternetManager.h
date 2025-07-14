#ifndef __INTERNET_MANAGER_H__
#define __INTERNET_MANAGER_H__

#include <cstdint>

#include "FileTransferManager.h"
#include "Logger.h"

namespace APP {

    class InternetManager{
        public:
            enum class InternetStatus: uint8_t{
                WifiConnected,
                WifiDisconnected,
                CloudConnected,
                CloudDisconnected
            };

            InternetManager()=default;
            static void execute();
            static InternetStatus getInternetStatus() { return currentInternetStatus; };
            static void setInternetStatus(InternetStatus status) { currentInternetStatus = status; }

            private:
            static InternetStatus currentInternetStatus;
            static Bsp::Logger InternetManagerLogger;
            static FileTransferManager inventoryFileManager_;


    };

}

#endif