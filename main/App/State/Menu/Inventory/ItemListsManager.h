#ifndef __ITEMS_LISTS_MANAGER_H__
#define __ITEMS_LISTS_MANAGER_H__
#include <array>
#include "Sqlite.h"
#include "Fatfs.h"
#include "RtcModule.h"

namespace APP {

    using ItemDescriptor = Bsp::product_obj_t;
    class ItemListsManager{
        private:
            std::array<ItemDescriptor, 30> itemContainer = {};
            uint32_t totalItemPrice;
            uint8_t currentIndex;
            //Sqlite inventoryItemDatabase;
        
        public:
            ItemListsManager()= default;
            //~ItemListsManager();
            void addItemToContainer( ItemDescriptor currentItem );
            ItemDescriptor getItemByIndex( uint8_t index );
            void setToTotalPrice ( uint32_t currentPrice ) { totalItemPrice = currentPrice; }
            void resetTotalPrice ( void ) { totalItemPrice = 0; }
            uint32_t getTotalPrice ( void ) { return totalItemPrice; }
            void clearItemContainer( void ) { itemContainer.fill(ItemDescriptor{}); }
            void saveItemListsToDatabase( void );
            void printItemContainerLists( void );
            void resetItemContainer();

    };

}



#endif