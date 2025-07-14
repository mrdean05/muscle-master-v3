#ifndef __TOTAL_PRICE_H__
#define __TOTAL_PRICE_H__

#include "ChildStateAnnex.h"
#include "LCDFrameDisplay.h"
#include "Inventory.h"

namespace APP{
    class TotalPrice: public StateTemp {
        private:
            
            LCDFrameDisplay* totalPriceScreen;
            Bsp::Logger totalPriceLogs; 
        public:
            explicit TotalPrice();
            void entryAction() override;
            void existAction() override;
            void handleEvent(Events::Event eventType) override; 
    };
}

#endif