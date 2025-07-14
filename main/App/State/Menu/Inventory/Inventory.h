#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include <array>

#include "ChildStateAnnex.h"
#include "LCDFrameDisplay.h"
#include "Sqlite.h"
#include "ItemListsManager.h"

namespace APP{

    using productDesc = Bsp::product_obj_t;
    class Inventory: public ChildState {

        public:
            enum class InventoryStates: uint8_t {
                Table,
                ProductDetails,
                TotalPrice
            };
            static inline uint8_t itemListCount = 0;
            explicit Inventory(State* parentState_);
            void entryAction() override;
            void existAction() override;
            void handleEvent(Events::Event eventType) override;
            static void setCurrentChildStateAnnex( InventoryStates childStateAnnex );
            static productDesc* searchCurrentItemWithBarcode( const char* barcode );
            static productDesc& getCurrentItem( void ) { return  currentItem; }
            static void addToInventoryTotalPrice(uint32_t price){ inventoryTotalPrice += price; };
            static void resetInventoryTotalPrice();
            static uint32_t getInventoryTotalPrice() { return inventoryTotalPrice; }
            static void addItemToInventoryContainer( productDesc* product );
            static void saveInventoryTransaction();


        private:
            static inline uint32_t inventoryTotalPrice = 0;
            static productDesc currentItem;
            static ItemListsManager itemListsManager_;
            
            LCDFrameDisplay* inventoryScreen;
            static Bsp::Logger inventoryLogs;
            static std::array<StateTemp*, 3> inventoryChildStates;
            static InventoryStates currentChildStateAnnex; 
            static Sqlite shopItemDatabase;
            void openShopItemsDatabase(); 
            void closeShopItemsDatabase();

    };
}

#endif