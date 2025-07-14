#include "HttpClient.h"
#include "esp_crt_bundle.h"


namespace APP {

    HttpClient::EventStatus HttpClient::httpEventHandleStatus = EventStatus::None;

    esp_err_t HttpClient::httpEventHandler(esp_http_client_event_t *evt){
        
        switch(evt->event_id) {
            case HTTP_EVENT_ERROR:
                httpEventHandleStatus = EventStatus::Error;
                break;
            case HTTP_EVENT_ON_CONNECTED:
                httpEventHandleStatus = EventStatus::Connected;
                break;
            case HTTP_EVENT_HEADER_SENT:
                httpEventHandleStatus = EventStatus::InProcess;
                break;
            case HTTP_EVENT_ON_DATA:
                //if (outputLen == 0 && evt->user_data) {
                // we are just starting to copy the output data into the use
                //memset(evt->user_data, 0, MAX_HTTP_OUTPUT_BUFFER);
                httpEventHandleStatus = EventStatus::OnData;
                break;
            case HTTP_EVENT_ON_FINISH:
                httpEventHandleStatus = EventStatus::Success;
                break;
            default:
                httpEventHandleStatus = EventStatus::Unknown;
                break;
        }

        return ESP_OK;
    }

    HttpClient::HttpClient(){
        esp_http_client_config_t config = {};
        config.url = "https://ndkfz1czq1.execute-api.us-east-1.amazonaws.com/dev/musclesqlitebucket/item2.db";
        config.event_handler = httpEventHandler;
        config.user_data = responseBuffer;
        //config.transport_type = HTTP_TRANSPORT_OVER_SSL;
        //config.skip_cert_common_name_check = true;
        //config.cert_pem = 
        config.crt_bundle_attach = esp_crt_bundle_attach;

        client = esp_http_client_init(&config);
    }

    bool HttpClient::httpPutMethod(const std::string& url, const char* data){
        esp_http_client_set_url(client, url.c_str());
        esp_http_client_set_method(client, HTTP_METHOD_PUT);
        esp_http_client_set_header(client, "Content-Type", "application/json");

        esp_http_client_set_post_field(client, data, strlen(data));
        esp_err_t err = esp_http_client_perform(client);
        if (err == ESP_OK) {
            printf("HTTP POST Status = %d, content_length = %lld\n",
                    esp_http_client_get_status_code(client),
                    esp_http_client_get_content_length(client));
                    return true;
        } else {
            printf("HTTP POST request failed: %s\n", esp_err_to_name(err));
            return false;
        }
        //esp_http_client_close(client);
    }

    bool HttpClient::httpClientOpen( const std::string& url, size_t contentLength ){

        esp_http_client_set_url(client, url.c_str());
        esp_http_client_set_method(client, HTTP_METHOD_PUT);
        esp_http_client_set_header(client, "Content-Type", "application/json");

        std::string lengthStr = std::to_string(contentLength);
        esp_http_client_set_header(client, "Content-Length", lengthStr.c_str());

        esp_err_t err = esp_http_client_open(client, contentLength);  // -1 = chunked encoding
        if (err != ESP_OK) {
            printf("HTTP Failed to open HTTP connection: %s\n", esp_err_to_name(err));
            return false;
        }
        return true;
    }

    bool HttpClient::httpClientWrite( const char* buffer, uint16_t len){
        int written = esp_http_client_write(client, buffer, len); 
        if (written < 0 || written != len) {
            printf("HTTP write error or partial write (%d/%d)\n", written, len);
            return false;
        }

        esp_http_client_fetch_headers(client); 

        int status_code = esp_http_client_get_status_code(client);
        if (status_code != 200 && status_code != 204) {
            printf("Unexpected HTTP response code: %d\n", status_code);
            return false;
        }

        return true;
    }

    bool HttpClient::httpClientClose( void ){
         // Optional, but useful to read HTTP status code
        esp_http_client_close(client);
        return true;
    }

}