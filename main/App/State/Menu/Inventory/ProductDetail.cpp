#include "ProductDetail.h"
#include "KeypadCharacter.h"
#include "StateEnums.h"
#include "Table.h"
#include "Inventory.h"
#include <cstring>


namespace APP{
    ProductDetail::ProductDetail():
    productDetailScreen(LCDFrameDisplay::getInstance()),
    productDetailLogs("Table")
    {}


    void ProductDetail::entryAction(){

    }

    void ProductDetail::existAction(){

    }

    void ProductDetail::handleEvent(Events::Event eventType){
        if (eventType == Events::Event::AlphaNumeric){
            char chPressed = KeypadCharacter::getPressedKey2();
            productDetailScreen->productDescriptionTextArea(chPressed);
            itemQuantity[itemQuantityIndex] = chPressed;
            itemQuantityIndex++;

        }

        else if (eventType == Events::Event::Enter){
            itemQuantity[itemQuantityIndex] = '\0';
            Bsp::product_obj_t& currentProduct = Inventory::getCurrentItem();
            strcpy(currentProduct.product_quantity, itemQuantity.data());
            uint16_t totalProductPrice = calculateProductItemWithQuantity(itemQuantity.data(), currentProduct.product_price);
            sprintf(currentProduct.total_price, "%d", totalProductPrice);
            Inventory::addToInventoryTotalPrice(static_cast<uint32_t>(totalProductPrice));
            productDetailScreen->productDescriptionEnterTextArea(currentProduct);
            Inventory::setCurrentChildStateAnnex(Inventory::InventoryStates::Table);
            Inventory::addItemToInventoryContainer(&currentProduct);
            currentProduct = {};
            itemQuantityIndex = 0;
            Inventory::itemListCount++;
        }

    }

    uint16_t ProductDetail::calculateProductItemWithQuantity(const char* quantity, const char* price){
        uint16_t qty = atoi(quantity);
        uint16_t prc = atoi(price);
        
        return qty * prc;
    }

}
