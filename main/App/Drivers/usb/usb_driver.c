#include <string.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "usb/usb_host.h"
#include "usb/hid_host.h"
#include "usb/hid_usage_keyboard.h"
#include "usb/hid_usage_mouse.h"

#include "usb_driver.h"


static const char *TAG = "USB_DRIVER";
#define BARCODE_MAX_LENGTH 13

/* GPIO Pin number for quit from example logic */
#define APP_QUIT_PIN                GPIO_NUM_0
#define KEYBOARD_ENTER_MAIN_CHAR    '\r'

const uint8_t keycode2ascii [57][2] = {
    {0, 0}, /* HID_KEY_NO_PRESS        */
    {0, 0}, /* HID_KEY_ROLLOVER        */
    {0, 0}, /* HID_KEY_POST_FAIL       */
    {0, 0}, /* HID_KEY_ERROR_UNDEFINED */
    {'a', 'A'}, /* HID_KEY_A               */
    {'b', 'B'}, /* HID_KEY_B               */
    {'c', 'C'}, /* HID_KEY_C               */
    {'d', 'D'}, /* HID_KEY_D               */
    {'e', 'E'}, /* HID_KEY_E               */
    {'f', 'F'}, /* HID_KEY_F               */
    {'g', 'G'}, /* HID_KEY_G               */
    {'h', 'H'}, /* HID_KEY_H               */
    {'i', 'I'}, /* HID_KEY_I               */
    {'j', 'J'}, /* HID_KEY_J               */
    {'k', 'K'}, /* HID_KEY_K               */
    {'l', 'L'}, /* HID_KEY_L               */
    {'m', 'M'}, /* HID_KEY_M               */
    {'n', 'N'}, /* HID_KEY_N               */
    {'o', 'O'}, /* HID_KEY_O               */
    {'p', 'P'}, /* HID_KEY_P               */
    {'q', 'Q'}, /* HID_KEY_Q               */
    {'r', 'R'}, /* HID_KEY_R               */
    {'s', 'S'}, /* HID_KEY_S               */
    {'t', 'T'}, /* HID_KEY_T               */
    {'u', 'U'}, /* HID_KEY_U               */
    {'v', 'V'}, /* HID_KEY_V               */
    {'w', 'W'}, /* HID_KEY_W               */
    {'x', 'X'}, /* HID_KEY_X               */
    {'y', 'Y'}, /* HID_KEY_Y               */
    {'z', 'Z'}, /* HID_KEY_Z               */
    {'1', '!'}, /* HID_KEY_1               */
    {'2', '@'}, /* HID_KEY_2               */
    {'3', '#'}, /* HID_KEY_3               */
    {'4', '$'}, /* HID_KEY_4               */
    {'5', '%'}, /* HID_KEY_5               */
    {'6', '^'}, /* HID_KEY_6               */
    {'7', '&'}, /* HID_KEY_7               */
    {'8', '*'}, /* HID_KEY_8               */
    {'9', '('}, /* HID_KEY_9               */
    {'0', ')'}, /* HID_KEY_0               */
    {KEYBOARD_ENTER_MAIN_CHAR, KEYBOARD_ENTER_MAIN_CHAR}, /* HID_KEY_ENTER           */
    {0, 0}, /* HID_KEY_ESC             */
    {'\b', 0}, /* HID_KEY_DEL             */
    {0, 0}, /* HID_KEY_TAB             */
    {' ', ' '}, /* HID_KEY_SPACE           */
    {'-', '_'}, /* HID_KEY_MINUS           */
    {'=', '+'}, /* HID_KEY_EQUAL           */
    {'[', '{'}, /* HID_KEY_OPEN_BRACKET    */
    {']', '}'}, /* HID_KEY_CLOSE_BRACKET   */
    {'\\', '|'}, /* HID_KEY_BACK_SLASH      */
    {'\\', '|'}, /* HID_KEY_SHARP           */  // HOTFIX: for NonUS Keyboards repeat HID_KEY_BACK_SLASH
    {';', ':'}, /* HID_KEY_COLON           */
    {'\'', '"'}, /* HID_KEY_QUOTE           */
    {'`', '~'}, /* HID_KEY_TILDE           */
    {',', '<'}, /* HID_KEY_LESS            */
    {'.', '>'}, /* HID_KEY_GREATER         */
    {'/', '?'} /* HID_KEY_SLASH           */
};

/**
 * @brief Key event
 */
typedef struct {
    enum key_state {
        KEY_STATE_PRESSED = 0x00,
        KEY_STATE_RELEASED = 0x01
    } state;
    uint8_t modifier;
    uint8_t key_code;
} key_event_t;


QueueHandle_t app_event_queue = NULL;
QueueHandle_t barcode_id_queue;

typedef struct {
    hid_host_device_handle_t hid_device_handle;
    hid_host_driver_event_t event;
    void *arg;
} lvgl_port_usb_hid_event_t;


// Buffer to store the barcode
static char barcode_buffer[BARCODE_MAX_LENGTH + 1] = {0}; // +1 for null terminator
static int barcode_index = 0;


/********************************************************************
 *              STATIC FUNCTION DECLARATION
 ********************************************************************/
void hid_host_device_callback(hid_host_device_handle_t hid_device_handle,
                              const hid_host_driver_event_t event,
                              void *arg);

void USBDriver_HIDHostInit ( void );
void hid_host_interface_callback(hid_host_device_handle_t HID_device_handle, const hid_host_interface_event_t event, void *arg);
static void hid_host_keyboard_report_callback(const uint8_t *const data, const int length);
static inline bool key_found(const uint8_t *const src, uint8_t key, unsigned int length);
static void key_event_callback(key_event_t *key_event);
static void process_barcode(const char *barcode);
static inline bool hid_keyboard_get_char(uint8_t modifier, uint8_t key_code, unsigned char *key_char);
static inline bool hid_keyboard_is_modifier_shift(uint8_t modifier);

/********************************************************************************************
 *                          STATIC FUNCTIONS
 ********************************************************************************************/

static void process_barcode(const char *barcode) {
    // Handle the barcode (e.g., print, send, or store)
    char *barcode_id = (char*) malloc(14*sizeof(char));
    if(barcode_id != NULL){
        // Copy the barcode into the allocated memory
        strcpy(barcode_id, barcode);
        //Add to queue
        xQueueSend(barcode_id_queue, barcode_id, 0);
        //printf("Scanned Barcode: %s\n", barcode);
        //Free memory
        free(barcode_id);
    }
}

void hid_host_device_callback(hid_host_device_handle_t hid_device_handle,
                              const hid_host_driver_event_t event,
                              void *arg)
{
    
    const lvgl_port_usb_hid_event_t msg = {
        .hid_device_handle = hid_device_handle,
        .event = event,
        .arg = NULL
    };

   xQueueSend(app_event_queue, &msg, 0);

}


void hid_host_interface_callback(hid_host_device_handle_t HID_device_handle, const hid_host_interface_event_t event, void *arg)
{
    uint8_t data[64] = { 0 };
    size_t data_length = 0;
    hid_host_dev_params_t dev_p;
    ESP_ERROR_CHECK( hid_host_device_get_params(HID_device_handle, &dev_p));

    switch (event) {
    case HID_HOST_INTERFACE_EVENT_INPUT_REPORT:
        ESP_ERROR_CHECK( hid_host_device_get_raw_input_report_data(HID_device_handle,
                         data,
                         64,
                         &data_length));

        if (HID_SUBCLASS_BOOT_INTERFACE == dev_p.sub_class) {
            if (HID_PROTOCOL_KEYBOARD == dev_p.proto) {
                hid_host_keyboard_report_callback(data, data_length);
            } 
        } 
        break;

    case HID_HOST_INTERFACE_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HID Device, protocol DISCONNECTED");
        ESP_ERROR_CHECK( hid_host_device_close(HID_device_handle) );
        break;
    case HID_HOST_INTERFACE_EVENT_TRANSFER_ERROR:
        ESP_LOGI(TAG, "HID Device, protocol TRANSFER_ERROR");
        break;
    default:
        ESP_LOGE(TAG, "HID Device, protocol handled event");
        break;
    }
}

static void hid_host_keyboard_report_callback(const uint8_t *const data, const int length)
{

    hid_keyboard_input_report_boot_t *kb_report = (hid_keyboard_input_report_boot_t *)data;

    if (length < sizeof(hid_keyboard_input_report_boot_t)) {
        return;
    }

    static uint8_t prev_keys[HID_KEYBOARD_KEY_MAX] = { 0 };
    key_event_t key_event;

    for (int i = 0; i < HID_KEYBOARD_KEY_MAX; i++) {
        if (kb_report->key[i] > HID_KEY_ERROR_UNDEFINED &&
                !key_found(prev_keys, kb_report->key[i], HID_KEYBOARD_KEY_MAX)) {
            key_event.key_code = kb_report->key[i];
            key_event.modifier = kb_report->modifier.val;
            key_event.state = KEY_STATE_PRESSED;
            key_event_callback(&key_event);
        }
    }
    memcpy(prev_keys, &kb_report->key, HID_KEYBOARD_KEY_MAX);
}

static inline bool key_found(const uint8_t *const src,
                             uint8_t key,
                             unsigned int length)
{
    for (unsigned int i = 0; i < length; i++) {
        if (src[i] == key) {
            return true;
        }
    }
    return false;
}

static void key_event_callback(key_event_t *key_event){
    unsigned char key_char;

    if (KEY_STATE_PRESSED == key_event->state) {
        if (hid_keyboard_get_char(key_event->modifier,
                                  key_event->key_code, &key_char)) {

            //hid_keyboard_print_char(key_char);
            if (key_char == KEYBOARD_ENTER_MAIN_CHAR) {
                // Barcode scan complete
                barcode_buffer[barcode_index] = '\0'; // Null-terminate the string
                process_barcode(barcode_buffer);

                // Reset the buffer for the next scan
                barcode_index = 0;
                memset(barcode_buffer, 0, sizeof(barcode_buffer));
            } else if (barcode_index < BARCODE_MAX_LENGTH) {
                // Append the character to the buffer
                barcode_buffer[barcode_index++] = key_char;
            } else {
                // Buffer overflow, reset
                ESP_LOGW(TAG, "Barcode overflow, resetting buffer.");
                barcode_index = 0;
                memset(barcode_buffer, 0, sizeof(barcode_buffer));
            }

        }
    }
}

static inline bool hid_keyboard_is_modifier_shift(uint8_t modifier)
{
    if (((modifier & HID_LEFT_SHIFT) == HID_LEFT_SHIFT) ||
            ((modifier & HID_RIGHT_SHIFT) == HID_RIGHT_SHIFT)) {
        return true;
    }
    return false;
}

static inline bool hid_keyboard_get_char(uint8_t modifier,
        uint8_t key_code,
        unsigned char *key_char)
{
    uint8_t mod = (hid_keyboard_is_modifier_shift(modifier)) ? 1 : 0;

    if ((key_code >= HID_KEY_A) && (key_code <= HID_KEY_SLASH)) {
        *key_char = keycode2ascii[key_code][mod];
    } else {
        // All other key pressed
        return false;
    }

    return true;
}

/*********************************************************************
 * FUNCTIONS DEFINITIONS
 *********************************************************************/

void USBDriver_initTask( void *arg ){

    barcode_id_queue =  *(QueueHandle_t*)arg;
    
    const gpio_config_t input_pin = {
        .pin_bit_mask = BIT64(APP_QUIT_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    ESP_ERROR_CHECK( gpio_config(&input_pin) );
    
    //ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1));
    //ESP_ERROR_CHECK(gpio_isr_handler_add(APP_QUIT_PIN, gpio_isr_cb, NULL));

    const usb_host_config_t host_config = {
        .skip_phy_setup = false,
        .intr_flags = ESP_INTR_FLAG_LEVEL1,
    };

    ESP_ERROR_CHECK( usb_host_install(&host_config) );

    USBDriver_HIDHostInit();
    vTaskDelay(20);


    while (gpio_get_level(APP_QUIT_PIN) != 0) {
        uint32_t event_flags;
        usb_host_lib_handle_events(portMAX_DELAY, &event_flags);

        // Release devices once all clients has deregistered
        if (event_flags & USB_HOST_LIB_EVENT_FLAGS_NO_CLIENTS) {
            usb_host_device_free_all();
            ESP_LOGI(TAG, "USB Event flags: NO_CLIENTS");
        }
        // All devices were removed
        if (event_flags & USB_HOST_LIB_EVENT_FLAGS_ALL_FREE) {
            ESP_LOGI(TAG, "USB Event flags: ALL_FREE");
        }
    }

    ESP_LOGI(TAG, "USB shutdown");
    // Clean up USB Host
    vTaskDelay(10); // Short delay to allow clients clean-up
    ESP_ERROR_CHECK( usb_host_uninstall());
    vTaskDelete(NULL);

}

void USBDriver_HIDHostInit ( void ){

    /*
    * HID host driver configuration
    * - create background task for handling low level event inside the HID driver
    * - provide the device callback to get new HID Device connection event
    */
    const hid_host_driver_config_t hid_host_driver_config = {
        .create_background_task = true,
        .task_priority = 7,
        .stack_size = 4096,
        .core_id = 1,
        .callback = hid_host_device_callback,
        .callback_arg = NULL
    };

    ESP_ERROR_CHECK( hid_host_install(&hid_host_driver_config) );
    printf("HID Host Initialized\n");
    app_event_queue = xQueueCreate(10, sizeof(lvgl_port_usb_hid_event_t));

    xTaskCreatePinnedToCore(USBDriver_HIDHostQueue, "start screen_task", 4 * 1024, NULL,  2, NULL, 1);
} 

void USBDriver_HIDHostQueue( void *args ){
    hid_host_dev_params_t dev_params;
    hid_host_device_handle_t hid_device_handle = NULL;
    lvgl_port_usb_hid_event_t msg; 
    
    while(1){
        if (xQueueReceive(app_event_queue, &msg, pdMS_TO_TICKS(50))){
            hid_device_handle = msg.hid_device_handle;
            hid_host_device_get_params(hid_device_handle, &dev_params);        
            ESP_LOGI(TAG, "Entered the queue\n");
            switch (msg.event) {
                case HID_HOST_DRIVER_EVENT_CONNECTED:  
                    ESP_LOGI(TAG, "Host Driver Connected");

                    const hid_host_device_config_t dev_config = {
                        .callback = hid_host_interface_callback,
                        .callback_arg = NULL
                    };

                    hid_device_handle = msg.hid_device_handle;

                    ESP_ERROR_CHECK( hid_host_device_open(hid_device_handle, &dev_config) );
                    if (HID_SUBCLASS_BOOT_INTERFACE == dev_params.sub_class) {
                        ESP_ERROR_CHECK( hid_class_request_set_protocol(hid_device_handle, HID_REPORT_PROTOCOL_BOOT));
                        if (HID_PROTOCOL_KEYBOARD == dev_params.proto) {
                        ESP_ERROR_CHECK( hid_class_request_set_idle(hid_device_handle, 0, 0));
                        }
                    }
                    ESP_ERROR_CHECK( hid_host_device_start(hid_device_handle) );
                break;
            
            default:
                break;
            
            }
        }
    }
}




