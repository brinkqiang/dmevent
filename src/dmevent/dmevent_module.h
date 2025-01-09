
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

#include <memory>
#include <atomic>
#include <vector>
#include <any>

#include "asio.hpp"

#include "dmutil.h"
#include "dmformat.h"
#include "type_traits.h"


class CDMEventModule :
	public std::enable_shared_from_this<CDMEventModule>
{

public:
	CDMEventModule();

	virtual ~CDMEventModule();

	virtual void Init(void);

	virtual bool Run(int event);

	virtual bool RunUntil();

	virtual void OnStop();

	template <typename... Args>
	void Post(Args&& ... args)
	{
		m_io_event.post(std::forward<Args>(args)...);
	}

	template <typename Func, typename... Args>
	auto Call(Func&& func, Args&&... args)
		-> typename std::invoke_result<Func, Args...>::type {
		using ReturnType = typename std::invoke_result<Func, Args...>::type;

		std::promise<ReturnType> resultPromise;
		auto resultFuture = resultPromise.get_future();

		m_io_event.post([&resultPromise, &func, &args...]() {
			try {
				if constexpr (std::is_void_v<ReturnType>) {
					std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
					resultPromise.set_value();
				}
				else {
					resultPromise.set_value(
						std::invoke(std::forward<Func>(func), std::forward<Args>(args)...));
				}
			}
			catch (...) {
				resultPromise.set_exception(std::current_exception());
			}
		});

		resultFuture.wait();
		return resultFuture.get();
	}

	template <auto Func, typename... Args>
	auto Call(Args&&... args)
		-> typename std::invoke_result<decltype(Func), Args...>::type {
		return Call(Func, std::forward<Args>(args)...);
	}

	asio::io_context& GetIO();

private:
	asio::io_context m_io_event;
	asio::io_context::work m_io_work;
	std::thread m_thread;
	asio::signal_set m_signals;
	std::atomic_bool m_stop;
public:
	using EventKey = std::string;

	template <typename... Args>
	auto ConvertArgsToString(Args&&... args) {
		// 使用递归转换，将 const char* 转换为 std::string
		return std::tuple<std::conditional_t<std::is_same_v<std::decay_t<Args>, const char*>, std::string, std::decay_t<Args>>...>(
			std::forward<Args>(args)...);
	}
	// 注册事件
	template <typename... Args>
	auto CallRouter(const EventKey& eventName, Args&&... args) {
		auto it = m_eventHandlers.find(eventName);
		if (it == m_eventHandlers.end()) {
			throw std::runtime_error("Event not found: " + eventName);
		}

		auto& handler = it->second;

		auto tupleArgs = ConvertArgsToString(std::forward<Args>(args)...);

		// 调用 handler 并返回结果
		using ReturnType = util::function_return_type_t<std::decay_t<decltype(handler)>>;

		ReturnType result = handler(std::make_any<decltype(tupleArgs)>(tupleArgs));

		return result;
	}

	// 调用函数工具
	template <typename Func, typename Tuple, size_t... Is>
	static auto CallHandlerImpl(Func&& func, Tuple& args, std::index_sequence<Is...>) {
		return func(std::get<Is>(args)...);
	}

	template <typename Func, typename Tuple>
	static auto CallHandler(Func&& func, Tuple&& args) {
		constexpr size_t ArgCount = std::tuple_size_v<std::decay_t<Tuple>>;
		return CallHandlerImpl(std::forward<Func>(func), args, std::make_index_sequence<ArgCount>{});
	}

	template <typename Func>
	void RegisterRouter(const EventKey& eventName, Func&& handler) {
		using ParametersType = util::function_parameters_t<Func>;
		using ReturnType = util::function_return_type_t<Func>;

		std::cout << "Registering event: " << eventName << std::endl;
		std::cout << "ParametersType: " << typeid(ParametersType).name() << std::endl;
		std::cout << "ReturnType: " << typeid(ReturnType).name() << std::endl;

		m_eventHandlers[eventName] = [handler = std::forward<Func>(handler)](auto args)->ReturnType {

			auto tupleArgs = std::any_cast<ParametersType>(args);
			return CallHandler(handler, tupleArgs);
		};
	}
private:
	using GenericHandler = std::function<std::any(std::any)>;
	std::unordered_map<EventKey, GenericHandler> m_eventHandlers;
};


std::shared_ptr<CDMEventModule> dmeventGetModule();

#define DMEVENT_INIT() dmevent_module->Init()
#define DMEVENT_BEGIN dmevent_module->Post([=]()
#define DMEVENT_END )

#define DMEVENT_RUN_UNTIL() dmevent_module->RunUntil()
#define DMEVENT_RUN(n) dmevent_module->Run(n)
