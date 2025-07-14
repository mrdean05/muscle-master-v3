
#include "ProgramController.h"

using namespace APP;

extern "C" void app_main(void)
{
    ProgramController programControl;
    ProgramController::config();
    ProgramController::execute();


}
