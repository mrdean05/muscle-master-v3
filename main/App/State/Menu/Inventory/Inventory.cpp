#include "Inventory.h"
#include "Table.h"
#include "ProductDetail.h"
#include "TotalPrice.h"
#include "Fatfs.h"
#include "RtcModule.h"


namespace APP {

    std::array<StateTemp*, 3> Inventory::inventoryChildStates = { nullptr, nullptr, nullptr };
    Inventory::InventoryStates Inventory::currentChildStateAnnex = Inventory::InventoryStates::Table;
    productDesc Inventory::currentItem = {};
    Bsp::Logger Inventory::inventoryLogs("Inventory");
    Sqlite Inventory::shopItemDatabase;
    ItemListsManager Inventory::itemListsManager_;

    Inventory::Inventory(State* parentState_):
    inventoryScreen(LCDFrameDisplay::getInstance())
    {
        parentState = parentState_;

        static Table tableChildState = Table();
        static ProductDetail productDetailChildState = ProductDetail();
        static TotalPrice totalPriceChildState = TotalPrice();
    
        inventoryChildStates[0] = &tableChildState;
        inventoryChildStates[1] = &productDetailChildState;
        inventoryChildStates[2] = &totalPriceChildState;

    }

    void Inventory::entryAction(){

        currentChildStateAnnex = InventoryStates::Table;

        inventoryLogs.info("Display Interventory buffer\n");
        inventoryScreen->launchInventoryTable();

        // Open inventory items database
        inventoryLogs.info("Open item database\n");
        openShopItemsDatabase();

    }

    void Inventory::existAction(){
        //Close data
        inventoryLogs.info("Close item database\n");
        closeShopItemsDatabase();
        if ( itemListCount != 0) { itemListsManager_.resetItemContainer(); }
        inventoryChildStates[static_cast<int>(currentChildStateAnnex)]->existAction();
        currentChildStateAnnex = InventoryStates::Table;
    }

    void Inventory::handleEvent(Events::Event eventType){
        if (currentChildStateAnnex == InventoryStates::Table){
            inventoryChildStates[static_cast<int>(currentChildStateAnnex)]->handleEvent(eventType);
        }
        else if (currentChildStateAnnex == InventoryStates::ProductDetails){
            // if eventType is alphanumeric or clear or 
            //if (eventType == Events::Event::AlphaNumeric || eventType == Events::Event::)
            inventoryChildStates[static_cast<int>(currentChildStateAnnex)]->handleEvent(eventType);
        }

        else if (currentChildStateAnnex == InventoryStates::TotalPrice){
            inventoryChildStates[static_cast<int>(currentChildStateAnnex)]->handleEvent(eventType);
        }

        else {
            inventoryLogs.info("Unknown");
        }
    }

    //ToDo Handle the below with some grace
    void Inventory::openShopItemsDatabase(){
        
        std::string itemDBPath;
        itemDBPath = std::string(Bsp::Fatfs::basePath) + "/items.db";        
        
        shopItemDatabase.openDataBase(itemDBPath);
    }

    void Inventory::closeShopItemsDatabase(){
        shopItemDatabase.closeDataBase();
    }

    void Inventory::setCurrentChildStateAnnex(InventoryStates childStateAnnex){
        currentChildStateAnnex = childStateAnnex;
    }

    void Inventory::addItemToInventoryContainer( productDesc* product ){
        itemListsManager_.addItemToContainer(*product);
    }

    productDesc* Inventory::searchCurrentItemWithBarcode( const char* barcode ){
        if (shopItemDatabase.searchItems(barcode, &currentItem) == 0){
            return &currentItem;
        }
        else {
            return NULL;
        }
    }

    void Inventory::saveInventoryTransaction(){

        itemListsManager_.printItemContainerLists();
        itemListsManager_.saveItemListsToDatabase();
        itemListsManager_.resetItemContainer();
        itemListCount = 0;
    }


    void Inventory::resetInventoryTotalPrice() { 
        itemListsManager_.setToTotalPrice(inventoryTotalPrice); 
        inventoryTotalPrice = 0; 
    }
}