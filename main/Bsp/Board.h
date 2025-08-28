#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdint.h>

namespace Bsp{

    class Board {

        private:
            static uint8_t macAddr[6];


        public:
            Board();
            static const char* getMacAddr();

    };

}




#endif