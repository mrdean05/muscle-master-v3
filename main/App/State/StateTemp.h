#ifndef __STATE_TEMP_H__
#define __STATE_TEMP_H__
#include "Events.h"

namespace APP{
    class StateTemp{
    public:
        virtual void entryAction() = 0;
        virtual void existAction() = 0;
        virtual void handleEvent(Events::Event eventType) = 0;
    };
}

#endif