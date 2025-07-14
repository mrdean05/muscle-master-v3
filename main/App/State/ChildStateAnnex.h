#ifndef __CHILD_STATE_ANEX_H__
#define __CHILD_STATE_ANEX_H__
#include "StateTemp.h"
#include "ChildState.h"

namespace APP{
    class ChildStateAnnex : public StateTemp {
        protected:
            ChildState* parentState; 
    };
};



#endif