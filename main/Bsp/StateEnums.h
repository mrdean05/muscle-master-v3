#ifndef __STATE_ENUMS_H__
#define __STATE_ENUMS_H__

#include <cstdint>

namespace Bsp{

    enum class statetype_t {
        LAUNCH_STATE    =   0x05,
        REGISTER_STATE,
        MENU_STATE,
        SLEEP_STATE,
        SHUTDOWN_STATE
    };  
    
    enum class childState_type {
        NO_CHILD_STATE,
        MENU_ENTRY,
        MENU_INVENTORY,
        MENU_CALCULATOR,
        MENU_SETTINGS,
        INVENTORY_TABLE,
        INVENTORY_ITEM_INFO,
        INVENTORY_TOTAL_COST,
        INVENTORY_PRINT_COST
    };

    enum class command_type {
        STATE = 0x10,
        CHILDSTATE,
        EVENT,
        CLEAR_GPIO
    };

    enum class EventType {
        EVT_USER_INPUT,         // User pressed a key
        EVT_CLEAR_CHAR,
        EVT_BARCODE_SCAN,       // Barcode scanner input received
        EVT_ENTER_PRESSED,      // Enter key pressed
        EVT_SHUTDOWN_REQUEST,   // Request to shutdown
        EVT_SLEEP_REQUEST       // Request to sleep        
    };

    typedef struct {
        char product_brand [50];
        char product_type [50];
        char product_price [10];
        char product_weight [10];
        char product_barcode_id[14];
        char product_quantity[10];
        char total_price[10];
    } product_obj_t;
    
    typedef struct {
        EventType event_type;
        union {
            char key_pressed;
            char text[24];
            product_obj_t product_data;
        } event_data;
    } Event;
    
    typedef struct {
        command_type command;
        union {
            statetype_t state_type;
            childState_type childstate; 
            Event event;
        } data;
    } spiMessagePayload;

}



#endif