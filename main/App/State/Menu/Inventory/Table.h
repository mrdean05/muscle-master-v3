#ifndef __TABLE_H__
#define __TABLE_H__

#include "ChildState.h"
#include "LCDFrameDisplay.h"
#include "Sqlite.h"
#include "StateEnums.h"
#include "Inventory.h"

namespace APP{
    class Table: public StateTemp {
        private:        
            LCDFrameDisplay* tableScreen;
            Sqlite* itemDB;
            Bsp::Logger tableLogs; 
            //static Bsp::product_obj_t currentProduct;

        public:
            explicit Table();
            void entryAction() override;
            void existAction() override;
            void handleEvent(Events::Event eventType) override; 
            //static Bsp::product_obj_t& getCurrentProduct() { return  currentProduct; }
    };
}

#endif