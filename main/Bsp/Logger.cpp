#include "Logger.h"

namespace Bsp {
    void Logger::esp_log_vprintf(esp_log_level_t level, const char* tag, const char* fmt, 
                                va_list args){
        esp_log_writev(level, tag, fmt, args);        
    }

    void Logger::info(const char* fmt, ...) const {
        va_list args;
        va_start(args, fmt);
        esp_log_vprintf(ESP_LOG_INFO, tag_, fmt, args);
        va_end(args);
    }

    void Logger::warn(const char* fmt, ...) const {
        va_list args;
        va_start(args, fmt);
        esp_log_vprintf(ESP_LOG_ERROR, tag_, fmt, args);
        va_end(args);
    }

    void Logger::error(const char* fmt, ...) const {
        va_list args;
        va_start(args, fmt);
        esp_log_vprintf(ESP_LOG_ERROR, tag_, fmt, args);
        va_end(args);
    }

    void Logger::debug(const char* fmt, ...) const {
        va_list args;
        va_start(args, fmt);
        esp_log_vprintf(ESP_LOG_DEBUG, tag_, fmt, args);
        va_end(args);
    }

}