#include "FileTransferManager.h"
#include "Fatfs.h"
#include "RtcModule.h"

namespace APP{

    #define BUFFER_SIZE 1024
    HttpClient FileTransferManager::fileTransferHttp;


    FileTransferManager::FileTransferManager(std::string& path){
        path_ = strdup(path.c_str());
    }

    bool FileTransferManager::transferBuffer(const char* data){
        
        // check if the path is empty, if not
        //std::string inventoryFilePath = std::string(Bsp::Fatfs::basePath) + "/trans";

        //fileUpload:
        //if (!isPathEmpty()){

        //}

        RtcModule::dateAndTime time = RtcModule::getTime();
        std::string dateTime = std::to_string(time.minutes); //+ std::to_string(time.seconds);
        
        std::string url = "https://ndkfz1czq1.execute-api.us-east-1.amazonaws.com/dev/musclesqlitebucket/" + dateTime + ".json";
        //const char* data = "This is my data";
        return fileTransferHttp.httpPutMethod(url, data);
            
    }

    /*

    bool FileTransferManager::openPathDir( void ){

        dir = opendir(path_);
        if (!dir) {
            printf("Error check directory\n");
            return false;
        }
        return true;
    }
    

    bool FileTransferManager::closePathDir( void ){
        closedir(dir);
        pathStatus = true;
        return true;
    }
    */

    bool FileTransferManager::isPathEmpty(){

        DIR *dir = opendir(path_);
        if (!dir) {
            printf("Error check directory\n");
            return false;
        }
        
        struct dirent *entry;
        std::string fullPath;

        while ((entry = readdir(dir)) != NULL){
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;            
            }
            fullPath = std::string(path_) + "/" + entry->d_name;
            struct stat st;
            if (stat(fullPath.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
                closedir(dir);
                return true;
            }
        }
        closedir(dir);
        return false;
    }

    /*

    std::string FileTransferManager::readFileName(){

        struct dirent *entry;
        std::string fullPath;

        while ((entry = readdir(dir)) != NULL) {
            // Skip "." and ".."
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            fullPath = std::string(path_) + "/" + entry->d_name;
            struct stat st;
            if (stat(fullPath.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
                printf("Found file: %s", fullPath.c_str());
                return std::string(fullPath);
            }
        }
        return std::string("");   
    }

    */

    bool FileTransferManager::fileExists(const std::string& fileName){
        
        DIR *dir = opendir(path_);
        if (!dir) {
            printf("Error check directory\n");
            return false;
        }
        
        struct dirent *entry;
        
        while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Compare the current entry name with the target file name
        if (fileName == entry->d_name) {
            // Check that it is a regular file
            std::string fullPath = std::string(path_) + "/" + fileName;
            struct stat st;
            if (stat(fullPath.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
                closedir(dir);
                return true;
                }
            }
        }
        
        closedir(dir);
        return false;
    }

    bool FileTransferManager::writeToFile(const std::string& filename, const char *content) {
        
        int fd = 0;

        list_dir(path_);
        
        std::string filepath = std::string(path_) + "/" + filename;

        if (fileExists(filename)){ 
            fd = open(filepath.c_str(), O_WRONLY | O_APPEND, 0); 
            printf("File append\n"); 

            if (fd < 0){
            printf("Error opening file for append\n");
            return false;
            }

            // Add comma before new content
            const char* comma = ",\n";
            write(fd, comma, strlen(comma));

        }
        else { 
            fd = open(filepath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0); 
            printf("File created and writing opening [\n"); 

            if (fd < 0){
            printf("Error creating file\n");
            return false;
            }  
            
            // Write opening [
            const char* open_bracket = "{\n    \"transactions\": [\n";
            write(fd, open_bracket, strlen(open_bracket));            

        }
            
        write(fd, content, strlen(content));
        fsync(fd);
        list_dir(path_);

        close(fd);
        return true;
    }


    bool FileTransferManager::writeToFile(const std::string& filename){

        
        list_dir(path_);
        std::string filepath = std::string(path_) + "/" + filename;

        if (fileExists(filename)){ 
            int fd = 0;
            fd = open(filepath.c_str(), O_WRONLY | O_APPEND, 0); 
            printf("File append\n"); 

            if (fd < 0){
            printf("Error opening file for append\n");
            return false;
            }

            // Add comma before new content
            const char* comma = "]\n}";
            write(fd, comma, strlen(comma));

            fsync(fd);
            close(fd);
            return true;

        }
        return false;

    }

    bool FileTransferManager::renameFile(const std::string& oldFilename, const std::string& newFilename) {
        
        std::string oldPath = std::string(path_) + "/" + oldFilename;
        std::string newPath = std::string(path_) + "/" + newFilename;
        
        if (rename(oldPath.c_str(), newPath.c_str()) == 0) {
            printf("Renamed file: %s -> %s\n", oldPath.c_str(), newPath.c_str());
            return true;
        } else {
            perror("Rename failed");
        }
        return false;
    }

    bool FileTransferManager::readFileIntoBuffer(const std::string& filename, char *buffer, size_t buffer_size) {
        
        std::string filePathName = std::string(path_) + "/" + filename;
        
        FILE *file = fopen(filePathName.c_str(), "r");
        if (file == NULL) {
            perror("Failed to open file");
            return false;
        }

        char line[256];
        buffer[0] = '\0'; // Start with empty string

        while (fgets(line, sizeof(line), file)) {
            // Check remaining space before appending
            if (strlen(buffer) + strlen(line) + 1 >= buffer_size) {
                printf("Buffer overflow prevented while reading file\n");
                fclose(file);
                return false;
            }

            strcat(buffer, line);
        }

        fclose(file);
        return true;
    }


    bool FileTransferManager::removeFile(const std::string& filename){

        std::string filePathName = std::string(path_) + "/" + filename;
            if (remove(filePathName.c_str()) == 0) {
                printf("File deleted: %s\n", filePathName.c_str());
                return true;
            } else {
                perror("Failed to delete file");
                return false;
            }

    }

    void FileTransferManager::list_dir(const char *path)
    {
        //ESP_LOGI(TAG, "Listing files in %s:", path);

        DIR *dir = opendir(path);
        if (!dir) {
            //ESP_LOGE(TAG, "Failed to open directory: %s", strerror(errno));
            return;
        }

        printf("%s:\n", path);
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            printf(
                "    %s: %s\n",
                (entry->d_type == DT_DIR)
                    ? "directory"
                    : "file     ",
                entry->d_name
            );
        }

        closedir(dir);
    }

    bool FileTransferManager::streamFileToCloud(const std::string& filename){

        bool status = true;
        std::string filePathName = std::string(path_) + "/" + filename;
        char buffer[BUFFER_SIZE];
        uint16_t bytesRead = 0;

        // Get file size
        struct stat st;
        if (stat(filePathName.c_str(), &st) != 0) {
            printf("Failed to stat file, %s\n", filePathName.c_str());
            return false;
        }

        int fd = open(filePathName.c_str(), O_RDONLY);
        if (fd < 0) {
            ESP_LOGE("FILE", "Failed to open file: %s", filePathName.c_str());
            return false;
        }

        RtcModule::dateAndTime time = RtcModule::getTime();
        std::string dateTime = std::to_string(time.minutes); //+ std::to_string(time.seconds);
        //std::string url = "https://ndkfz1czq1.execute-api.us-east-1.amazonaws.com/dev/musclesqlitebucket/" + dateTime + ".json";
        std::string url = "https://smart-calculator-backend.onrender.com/inventory";// + dateTime + ".json";
        
        if(!fileTransferHttp.httpClientOpen(url, st.st_size)) { 
            status = false;
            goto cleanup;
        }
        
        while((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0){
            if(!fileTransferHttp.httpClientWrite(buffer, bytesRead))
            {
                printf("Error here\n");
                status = false;
                break;
            }
        }

    cleanup:
        fileTransferHttp.httpClientClose();
        close(fd);
        return status;
    }


}