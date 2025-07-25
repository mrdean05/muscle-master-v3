#include <ItemListsManager.h>
#include "Fatfs.h"
#include "cJSON.h"
#include "SalesFileManager.h"

namespace APP{

    //ItemListsManager::ItemListsManager():inventoryItemDatabase(){}
    void ItemListsManager::addItemToContainer( ItemDescriptor currentItem ){
        itemContainer[currentIndex] = currentItem;
        currentIndex++;
    }

    void ItemListsManager::saveItemListsToDatabase( void ){
        std::string itemPath;
        
        RtcModule::dateAndTime time = RtcModule::getTime();
        std::string dateTime = std::to_string(time.minutes) + std::to_string(time.seconds);
        
        std::string transactionId = "sale";
        
        std::string totalPriceStr = std::to_string(totalItemPrice);
        
        cJSON *root = NULL;
        cJSON *transaction_array = NULL;

        root = cJSON_CreateObject();
        cJSON_AddItemToObject(root, "type", cJSON_CreateString(transactionId.c_str()));
        cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(dateTime.c_str()));
        cJSON_AddItemToObject(root, "totalAmount", cJSON_CreateString(totalPriceStr.c_str())); // make it integer
        //cJSON_AddItemToObject(root, "transactions", transcations = cJSON_CreateArray());
        transaction_array = cJSON_CreateArray();


        //  itemPath = std::string(Bsp::Fatfs::basePath) + "/trans" + "/file" + std::to_string(time.minutes) + std::to_string(time.seconds) + ".db";
        //inventoryItemDatabase.openDataBase(itemPath);    
        //inventoryItemDatabase.createInventoryItemTable();

        for (uint8_t i = 0; i < currentIndex; i++){
            //inventoryItemDatabase.addItemToInventoryDB(&itemContainer[i]);
            cJSON *product_json  = cJSON_CreateObject();
            //cJSON_AddStringToObject(product_json , "brand", itemContainer[i].product_brand);
            //cJSON_AddStringToObject(product_json , "type", itemContainer[i].product_type);
            //cJSON_AddStringToObject(product_json , "weight", itemContainer[i].product_weight);
            cJSON_AddStringToObject(product_json , "barcodeId", itemContainer[i].product_barcode_id);
            cJSON_AddStringToObject(product_json , "quantity", itemContainer[i].product_quantity); //int
            cJSON_AddStringToObject(product_json , "price", itemContainer[i].total_price); // float

            cJSON_AddItemToArray(transaction_array, product_json);
        }

        // Add array to root
        cJSON_AddItemToObject(root, "items", transaction_array);

        //inventoryItemDatabase.closeDataBase();
        // Print the JSON string
        char *json_string = cJSON_Print(root);
        printf("%s\n", json_string);

        SalesFileManager::doOperation(SalesFileManager::Ops::Write, json_string);

        // Clean up
        free(json_string);
        cJSON_Delete(root);

        totalItemPrice = 0;
       
    }

    void ItemListsManager::resetItemContainer( void ){
        clearItemContainer();
        resetTotalPrice();
        currentIndex = 0;
    }


    void ItemListsManager::printItemContainerLists( void ){
        printf("Items in container: \n");
        for (uint8_t i = 0; i <= currentIndex; i++){
            printf("%s\n", itemContainer[i].product_type);
        }
    }

}