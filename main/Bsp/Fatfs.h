#ifndef __FATFS_H__
#define __FATFS_H__

#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"

namespace Bsp{
    class Fatfs{
        private:
            //inline static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
            Fatfs();
        public:
           inline static constexpr const char* basePath = "/data"; 
           static Fatfs* getInstance(void);
    };
}



#endif