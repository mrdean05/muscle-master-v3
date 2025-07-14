#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "esp_log.h"
#include <cstdarg>

namespace Bsp {
    class Logger {
        
        private:
            const char* tag_;
            static void esp_log_vprintf(esp_log_level_t level, const char* tag, const char* fmt, va_list args);

        public:
            explicit Logger (const char* tag): tag_(tag){};
            void info(const char* fmt, ...) const;
            void warn(const char* fmt, ...) const;
            void debug(const char* fmt, ...) const;
            void error(const char* fmt, ...) const;
    };
}

#endif