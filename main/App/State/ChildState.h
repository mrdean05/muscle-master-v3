#ifndef __CHILD_STATE_H__
#define __CHILD_STATE_H__
#include "StateTemp.h"
#include "State.h"

namespace APP{
    class ChildState : public StateTemp {
        protected:
            State* parentState; 
    };
};



#endif