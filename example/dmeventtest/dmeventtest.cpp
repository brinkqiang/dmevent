
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

		module->RegisterRouter("int", [](const int& name, const int& value) ->int
			{
				fmt::print("event: {} {}\n", name, value);

				return name + value;
			});

        module->RegisterRouter("test", [](const std::string& name, const std::string& value) ->std::string
        {
            fmt::print("event: {} {}\n", name, value);

            return name + value;
        });
        module->RegisterRouter("test2", [](const std::string& name, const std::string& value, int nID) ->std::string
        {
            fmt::print("event: {} {} {}\n", name, value, nID);
            return name + value + std::to_string(nID);
        });
		auto ret4 = module->CallRouter("int", 1, 2);

        auto ret2 = module->CallRouter("test", "hello", "world");

        auto ret3 = module->CallRouter("test2", "hello", "world", 1);
     
        fmt::print("{} + {} = {}\n", std::any_cast<int>(ret4), std::any_cast<std::string>(ret2), std::any_cast<std::string>(ret3));
    }

    return 0;
}
