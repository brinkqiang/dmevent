
#include "dmevent.h"
#include "dmformat.h"

int Add(int a, int b) {
	return a + b;
}

int main(int argc, char* argv[])
{
    std::shared_ptr<CDMEventModule> module = dmeventGetModule();

    if (module)
    {
        module->Init();
        module->Post([module]()
        {
            fmt::print("---------------------------------------------------------------\n");
            fmt::print("{} dmevent loop {} ...\n", DMGetExeName(), "running");
            fmt::print("---------------------------------------------------------------\n");
        });

		int x = 100;
		int y = 88;
		int z = 99;
        module->Post([module, x, y , z]()
        {
            fmt::print("---------------------------------------------------------------\n");
            fmt::print("{} {} {} ...\n", x, y, z);
            fmt::print("---------------------------------------------------------------\n");
        });

        std::this_thread::sleep_for(std::chrono::seconds(3));

        auto ret = module->Call<Add>( 1, 2);
        fmt::print("{} + {} = {}\n", 1, 2, ret);
    }


    return 0;
}
