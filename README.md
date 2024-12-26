# dmevent

Copyright (c) 2013-2018 brinkqiang (brink.qiang@gmail.com)

[![dmevent](https://img.shields.io/badge/brinkqiang-dmevent-blue.svg?style=flat-square)](https://github.com/brinkqiang/dmevent)
[![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://github.com/brinkqiang/dmevent/blob/master/LICENSE)
[![blog](https://img.shields.io/badge/Author-Blog-7AD6FD.svg)](https://brinkqiang.github.io/)
[![Open Source Love](https://badges.frapsoft.com/os/v3/open-source.png)](https://github.com/brinkqiang)
[![GitHub stars](https://img.shields.io/github/stars/brinkqiang/dmevent.svg?label=Stars)](https://github.com/brinkqiang/dmevent) 
[![GitHub forks](https://img.shields.io/github/forks/brinkqiang/dmevent.svg?label=Fork)](https://github.com/brinkqiang/dmevent)

## Build status
| [Linux][lin-link] | [Mac][mac-link] | [Windows][win-link] |
| :---------------: | :----------------: | :-----------------: |
| ![lin-badge]      | ![mac-badge]       | ![win-badge]        |

[lin-badge]: https://github.com/brinkqiang/dmevent/workflows/linux/badge.svg "linux build status"
[lin-link]:  https://github.com/brinkqiang/dmevent/actions/workflows/linux.yml "linux build status"
[mac-badge]: https://github.com/brinkqiang/dmevent/workflows/mac/badge.svg "mac build status"
[mac-link]:  https://github.com/brinkqiang/dmevent/actions/workflows/mac.yml "mac build status"
[win-badge]: https://github.com/brinkqiang/dmevent/workflows/win/badge.svg "win build status"
[win-link]:  https://github.com/brinkqiang/dmevent/actions/workflows/win.yml "win build status"

## Intro
dmevent
```cpp
#include "dmevent_module.h"

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

        module->RunUntil();
    }

    return 0;
}
```
## Contacts

## Thanks
