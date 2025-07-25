#ifndef __SALES_H__
#define __SALES_H__

#include <array>

#include "ChildStateAnnex.h"
#include "LCDFrameDisplay.h"
#include "Sqlite.h"
#include "ItemListsManager.h"

namespace APP{

    using productDesc = Bsp::product_obj_t;
    class Sales: public ChildState {

        public:
            enum class SalesStates: uint8_t {
                Table,
                ProductDetails,
                TotalPrice
            };
            static inline uint8_t itemListCount = 0;
            explicit Sales(State* parentState_);
            void entryAction() override;
            void existAction() override;
            void handleEvent(Events::Event eventType) override;
            static void setCurrentChildStateAnnex( SalesStates childStateAnnex );
            static productDesc* searchCurrentItemWithBarcode( const char* barcode );
            static productDesc& getCurrentItem( void ) { return  currentItem; }
            static void addToSalesTotalPrice(uint32_t price){ salesTotalPrice += price; };
            static void resetSalesTotalPrice();
            static uint32_t getSalesTotalPrice() { return salesTotalPrice; }
            static void addItemToSalesContainer( productDesc* product );
            static void saveSalesTransaction();


        private:
            static inline uint32_t salesTotalPrice = 0;
            static productDesc currentItem;
            static ItemListsManager itemListsManager_;
            
            LCDFrameDisplay* salesScreen;
            static Bsp::Logger salesLogs;
            static std::array<StateTemp*, 3> salesChildStates;
            static SalesStates currentChildStateAnnex; 
            static Sqlite shopItemDatabase;
            void openShopItemsDatabase(); 
            void closeShopItemsDatabase();

    };
}

#endif