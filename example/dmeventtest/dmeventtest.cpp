
#include "dmevent.h"

int main( int argc, char* argv[] ) {

    Idmevent* module = dmeventGetModule();
    if (module)
    {
        module->Test();
        module->Release();
    }
    return 0;
}
