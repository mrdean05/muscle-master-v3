#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include <cstdint>
#include <string>

#include "esp_http_client.h"
#include "esp_event.h"

namespace APP{

    class HttpClient {

        public:
            enum class EventStatus: uint8_t {
                None,
                Connected,
                InProcess,
                OnData,
                Success,
                Failed,
                Error,
                Unknown
            };
            
            HttpClient();
            bool httpPutMethod(const std::string& url, const char* data);
            bool httpClientOpen( const std::string& url, size_t contentLength );
            bool httpClientWrite( const char* buffer, uint16_t len);
            bool httpGetStatusCode(void);
            bool httpClientClose( void );

            static EventStatus getHttpEventStatus( void ) { return httpEventHandleStatus; }
            static void setHttpEventStatus( EventStatus eventType ) { httpEventHandleStatus =  eventType; }

        private:
            
            char responseBuffer[2048];
            esp_http_client_handle_t client;
            static EventStatus httpEventHandleStatus;
            static esp_err_t httpEventHandler(esp_http_client_event_t *evt);
            
            

    };

}

#endif