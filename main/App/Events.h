#ifndef __EVENTS_H__
#define __EVENTS_H__
#include <cstdint>

namespace APP {
    class Events{
        public:
            enum class Event: uint8_t {
                Launch,
                Menu,
                AlphaNumeric,
                Clear,
                Allclear,
                UpArrow,
                DownArrow,
                LeftArrow,
                RightArrow,
                Cashin,
                Cashout,
                Enter,
                Barcode,
                Null
            };
            Events() = default;
            static Event getEventFromChar (char ch); 
    };

}

#endif 