#ifndef __FILE_TRANSFER_MANAGER_H__
#define __FILE_TRANSFER_MANAGER_H__

#include "HttpClient.h"
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

namespace APP{
    class FileTransferManager{
        public:
            explicit FileTransferManager(std::string& path);
            static bool transferBuffer(const char* data);
            //bool openPathDir( void );
            //bool closePathDir( void );
            bool fileExists(const std::string& fileName);
            //bool isPathOpen( void ){ return pathStatus; }
            bool writeToFile(const std::string& filename, const char *content);
            bool writeToFile(const std::string& filename);
            bool renameFile(const std::string& oldFilename, const std::string& newFilename);
            bool removeFile(const std::string& filename);
            bool readFileIntoBuffer(const std::string& filename, char *buffer, size_t buffer_size);
            bool streamFileToCloud(const std::string& filename);
            void list_dir(const char *path);

        private:
            //DIR *dir;
            char* path_;
            //bool pathStatus;
            //std::array<char, 1024> inventoryfileBuffer = {};
            static HttpClient fileTransferHttp;
            bool isPathEmpty();
            std::string readFileName();
    };
}

#endif