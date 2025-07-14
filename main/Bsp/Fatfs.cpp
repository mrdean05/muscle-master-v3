#include "Fatfs.h"

#include "esp_log.h"
#include <sys/stat.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

namespace Bsp {

    wl_handle_t s_wl_handle = WL_INVALID_HANDLE;
    const char* basePath = "/data";


    Fatfs::Fatfs(){

        const esp_vfs_fat_mount_config_t mount_config = {
        .format_if_mount_failed = true,
            .max_files = 5,
            .allocation_unit_size = 4096
        };

        ESP_ERROR_CHECK(esp_vfs_fat_spiflash_mount_rw_wl(basePath, "storage", &mount_config, &s_wl_handle));
    }

    Fatfs* Fatfs::getInstance(){
        static Fatfs fatfsFile;
        return &fatfsFile;
    }

}