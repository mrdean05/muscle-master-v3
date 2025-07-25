#include "Sales.h"
#include "Table.h"
#include "ProductDetail.h"
#include "TotalPrice.h"
#include "Fatfs.h"
#include "RtcModule.h"


namespace APP {

    std::array<StateTemp*, 3> Sales::salesChildStates = { nullptr, nullptr, nullptr };
    Sales::SalesStates Sales::currentChildStateAnnex = Sales::SalesStates::Table;
    productDesc Sales::currentItem = {};
    Bsp::Logger Sales::salesLogs("Sales");
    Sqlite Sales::shopItemDatabase;
    ItemListsManager Sales::itemListsManager_;

    Sales::Sales(State* parentState_):
    salesScreen(LCDFrameDisplay::getInstance())
    {
        parentState = parentState_;

        static Table tableChildState = Table();
        static ProductDetail productDetailChildState = ProductDetail();
        static TotalPrice totalPriceChildState = TotalPrice();
    
        salesChildStates[0] = &tableChildState;
        salesChildStates[1] = &productDetailChildState;
        salesChildStates[2] = &totalPriceChildState;

    }

    void Sales::entryAction(){

        currentChildStateAnnex = SalesStates::Table;

        salesLogs.info("Display Interventory buffer\n");
        salesScreen->launchSalesTable();

        // Open sales items database
        salesLogs.info("Open item database\n");
        openShopItemsDatabase();

    }

    void Sales::existAction(){
        //Close data
        salesLogs.info("Close item database\n");
        closeShopItemsDatabase();
        if ( itemListCount != 0) { itemListsManager_.resetItemContainer(); }
        salesChildStates[static_cast<int>(currentChildStateAnnex)]->existAction();
        currentChildStateAnnex = SalesStates::Table;
    }

    void Sales::handleEvent(Events::Event eventType){
        if (currentChildStateAnnex == SalesStates::Table){
            salesChildStates[static_cast<int>(currentChildStateAnnex)]->handleEvent(eventType);
        }
        else if (currentChildStateAnnex == SalesStates::ProductDetails){
            // if eventType is alphanumeric or clear or 
            //if (eventType == Events::Event::AlphaNumeric || eventType == Events::Event::)
            salesChildStates[static_cast<int>(currentChildStateAnnex)]->handleEvent(eventType);
        }

        else if (currentChildStateAnnex == SalesStates::TotalPrice){
            salesChildStates[static_cast<int>(currentChildStateAnnex)]->handleEvent(eventType);
        }

        else {
            salesLogs.info("Unknown");
        }
    }          

    //ToDo Handle the below with some grace
    void Sales::openShopItemsDatabase(){
        
        std::string itemDBPath;
        itemDBPath = std::string(Bsp::Fatfs::basePath) + "/items.db";        
        
        shopItemDatabase.openDataBase(itemDBPath);
    }

    void Sales::closeShopItemsDatabase(){
        shopItemDatabase.closeDataBase();
    }

    void Sales::setCurrentChildStateAnnex(SalesStates childStateAnnex){
        currentChildStateAnnex = childStateAnnex;
    }

    void Sales::addItemToSalesContainer( productDesc* product ){
        itemListsManager_.addItemToContainer(*product);
    }

    productDesc* Sales::searchCurrentItemWithBarcode( const char* barcode ){
        if (shopItemDatabase.searchItems(barcode, &currentItem) == 0){
            return &currentItem;
        }
        else {
            return NULL;
        }
    }

    void Sales::saveSalesTransaction(){

        itemListsManager_.printItemContainerLists();
        itemListsManager_.saveItemListsToDatabase();
        itemListsManager_.resetItemContainer();
        itemListCount = 0;
    }


    void Sales::resetSalesTotalPrice() { 
        itemListsManager_.setToTotalPrice(salesTotalPrice); 
        salesTotalPrice = 0; 
    }
}