#ifndef __WIFI_MANAGER_H__
#define __WIFI_MANAGER_H__

#include "esp_event.h"
#include "esp_wifi.h"

namespace APP{
    class WiFiManager{
        private:
            static inline wifi_init_config_t wifi_cfg;
            static inline esp_event_handler_instance_t instance_any_id;
            static inline esp_event_handler_instance_t instance_got_ip;
            static void wifiEventHandler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data);
        public:
            WiFiManager() = default;
            static void initialize();
            //static void connect();
            //static void disconnect();
    };
}


#endif