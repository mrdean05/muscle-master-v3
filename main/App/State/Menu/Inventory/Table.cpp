#include <string>

#include "Table.h"
#include "ProgramController.h"
#include "StateEnums.h"
#include "Inventory.h"

namespace APP{


    Table::Table():
    tableScreen(LCDFrameDisplay::getInstance()),
    tableLogs("Table")
    {}


    void Table::entryAction(){

    }

    void Table::existAction(){

    }

    void Table::handleEvent(Events::Event eventType){
        if (eventType == Events::Event::Barcode){
                
            //get data from barcode
            const char* barcode = ProgramController::getBarcodeData().c_str();

            // search data from the database
            //if found, show product item screen and set State to ProductDetails
            Bsp::product_obj_t* itemFound = Inventory::searchCurrentItemWithBarcode(barcode); 
            if( itemFound != NULL ){
                tableScreen->productDisplay(*itemFound);
                Inventory::setCurrentChildStateAnnex(Inventory::InventoryStates::ProductDetails);
            }

        }
        else if (eventType == Events::Event::AlphaNumeric){
            //get the data from the key
        }

        else if ((eventType == Events::Event::UpArrow)||(eventType == Events::Event::DownArrow)){
            tableScreen->sendInput(KeypadCharacter::getPressedKey2());
        }

        else if (eventType == Events::Event::Enter){
            if (Inventory::itemListCount != 0){
                Inventory::setCurrentChildStateAnnex(Inventory::InventoryStates::TotalPrice);
                tableScreen->displayTotalPrice(std::to_string(Inventory::getInventoryTotalPrice()));
                Inventory::resetInventoryTotalPrice();
                Inventory::itemListCount = 0;
            }
        }
    }

}
