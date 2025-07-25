#include "TotalPrice.h"
#include "Sales.h"


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
            Sales::setCurrentChildStateAnnex(Sales::SalesStates::Table);
            Sales::saveSalesTransaction();
            totalPriceScreen->returnTable();
        }
    }

}
