#include "TotalPrice.h"
#include "Inventory.h"


namespace APP{
    TotalPrice::TotalPrice():
    totalPriceScreen(LCDFrameDisplay::getInstance()),
    totalPriceLogs("Table")
    {
    }

    void TotalPrice::entryAction(){

    }

    void TotalPrice::existAction(){
        
    }

    void TotalPrice::handleEvent(Events::Event eventType){
        if ((eventType == Events::Event::Enter) || (eventType == Events::Event::Cashin)){
            Inventory::setCurrentChildStateAnnex(Inventory::InventoryStates::Table);
            Inventory::saveInventoryTransaction();
            totalPriceScreen->returnTable();
        }
    }

}
