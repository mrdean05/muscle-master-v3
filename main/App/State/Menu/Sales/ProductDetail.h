#ifndef __PRODUCT_DETAIL_H__
#define __PRODUCT_DETAIL_H__

#include "ChildState.h"
#include "LCDFrameDisplay.h"
#include "Sales.h"
#include <array>

namespace APP{
    class ProductDetail: public StateTemp {
        private:
            LCDFrameDisplay* productDetailScreen;
            Bsp::Logger productDetailLogs;
            std::array<char, 7> itemQuantity;
            uint8_t itemQuantityIndex = 0;


        public:
            ProductDetail();
            void entryAction() override;
            void existAction() override;
            void handleEvent(Events::Event eventType) override; 
            uint16_t calculateProductItemWithQuantity(const char* quantity, const char* price);
    };
}

#endif