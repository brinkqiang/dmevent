
// Copyright (c) 2018 brinkqiang (brink.qiang@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "dmevent_module.h"

Cdmevent_module::Cdmevent_module()
    : m_io_work(m_io_event)
{

}

Cdmevent_module::~Cdmevent_module()
{

}

void Cdmevent_module::Init(void)
{
    auto self(shared_from_this());
    Post([this, self]()
    {
        fmt::print("---------------------------------------------------------------\n");
        fmt::print("{} dmevent loop {} ...\n", DMGetExeName(), "init");
        fmt::print("---------------------------------------------------------------\n");
    });
}

bool Cdmevent_module::Run(int event)
{
    int nEvent = m_io_event.poll_one();

    if (0 == nEvent)
    {
        return false;
    }

    for (int i=0; i < event; i++)
    {
        int nEvent = m_io_event.poll_one();

        if (0 == nEvent)
        {
            return true;
        }
    }

    return true;
}

asio::io_context& Cdmevent_module::GetIO()
{
    return m_io_event;
}

std::shared_ptr<Cdmevent_module> dmeventGetModule()
{
    return std::make_shared<Cdmevent_module>();
}
