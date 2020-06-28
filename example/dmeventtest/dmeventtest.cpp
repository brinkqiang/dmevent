
#include "dmevent_module.h"

int main( int argc, char* argv[] )
{
    std::shared_ptr<Cdmevent_module> module = dmeventGetModule();

    if (module)
    {
        module->Init();
        module->Run();
    }

    return 0;
}
