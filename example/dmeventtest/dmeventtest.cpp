
#include "dmevent_module.h"

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

		module->RegisterRouter("/getid", [](const int& a, const int& b) ->int
			{
				fmt::print("getid: {} {}\n", a, b);

				return a + b;
			});

        module->RegisterRouter("/helloworld", [](const std::string& name, const std::string& value) ->std::string
        {
            fmt::print("helloworld: {} {}\n", name, value);

            return name + value;
        });
        module->RegisterRouter("/helloworld_getid", [](const std::string& name, const std::string& value, int nID) ->std::string
        {
            fmt::print("helloworld_getid: {} {} {}\n", name, value, nID);
            return name + value + std::to_string(nID);
        });

		auto router_ret = module->CallRouter("/helloworld", "hello", "world");

		auto router_ret2 = module->CallRouter("/getid", 1, 2);

        auto router_ret3 = module->CallRouter("/helloworld_getid", "hello", "world", 3);
     
        fmt::print("{} + {} = {}\n", std::any_cast<std::string>(router_ret), std::any_cast<int>(router_ret2), std::any_cast<std::string>(router_ret3));
    }

    return 0;
}
