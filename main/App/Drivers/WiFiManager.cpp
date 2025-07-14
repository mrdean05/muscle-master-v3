#include "WiFiManager.h"
#include "InternetManager.h"

#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "NvsFlashManager.h"

#include <cstring>


namespace APP {

    using WifiCreds = Bsp::NvsFlashManager; 

    const char* ssid; //= "geo-fence";
    const char* passwd; //= "geo-fence";

    void WiFiManager::initialize(){
        //Initialise NVS
        
        //esp_err_t ret;
        /*
        = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
        }

        ESP_ERROR_CHECK(ret);
        */
        
        WifiCreds::open();
        WifiCreds::wifiCredentials creds = WifiCreds::getwifiCredentials();
        WifiCreds::close();

        ssid = creds.ssid.c_str();
        passwd = creds.passwd.c_str();

        printf("SSID: %s\n", ssid);
        printf("Passwd: %s\n", passwd);

        ESP_ERROR_CHECK(esp_netif_init());
        ESP_ERROR_CHECK(esp_event_loop_create_default());
        esp_netif_create_default_wifi_sta();


        ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifiEventHandler, NULL, &instance_any_id));
        ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifiEventHandler, NULL, &instance_got_ip));

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        memcpy( &wifi_cfg, &cfg, sizeof(wifi_init_config_t));
        ESP_ERROR_CHECK(esp_wifi_init(&wifi_cfg));
        
        //ESP_ERROR_CHECK(esp_wifi_start());
    
        static wifi_config_t wifi_config = {};
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        wifi_config.sta.sae_pwe_h2e = WPA3_SAE_PWE_BOTH;
        strcpy(reinterpret_cast<char*>(wifi_config.sta.sae_h2e_identifier), "");
        strcpy(reinterpret_cast<char*>(wifi_config.sta.ssid), ssid);
        strcpy(reinterpret_cast<char*>(wifi_config.sta.password), passwd);

        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
        ESP_ERROR_CHECK(esp_wifi_start());

    }

    void WiFiManager::wifiEventHandler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data){
        if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
            esp_wifi_connect();
        } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
            InternetManager::setInternetStatus(InternetManager::InternetStatus::WifiDisconnected);
            vTaskDelay(2000);
            esp_wifi_connect();
        } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){
            InternetManager::setInternetStatus(InternetManager::InternetStatus::WifiConnected);
        }
    }
}