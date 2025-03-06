
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

#include <memory>               // for std::shared_ptr, std::make_shared
#include <atomic>               // for std::once_flag, std::call_once
#include <unordered_map>        // for std::unordered_map
#include <string>               // for std::string
#include <functional>           // for std::function
#include <tuple>                // for std::tuple
#include <type_traits>          // for std::is_same_v, std::decay_t
#include <any>                  // for std::any, std::any_cast
#include <stdexcept>            // for std::runtime_error
#include <mutex>				
#include "dmtype_traits.h"


class CDMRouterModule :
	public std::enable_shared_from_this<CDMRouterModule>
{

public:
	CDMRouterModule() {}

	virtual ~CDMRouterModule() {}

public:
	using EventKey = std::string;

	template <typename... Args>
	static auto ConvertArgsToString(Args&&... args) {
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

		m_eventHandlers[eventName] = [handler = std::forward<Func>(handler)](auto args) {

			auto tupleArgs = std::any_cast<ParametersType>(args);

			return CallHandler(handler, tupleArgs);
		};
	}
private:
	using GenericHandler = std::function<std::any(std::any)>;
	std::unordered_map<EventKey, GenericHandler> m_eventHandlers;
};


inline std::shared_ptr<CDMRouterModule> dmrouterGetModule()
{
	static std::once_flag m_oOnce;
	static std::shared_ptr<CDMRouterModule> module;
	std::call_once(m_oOnce, [&]()
		{
			module = std::make_shared<CDMRouterModule>();
		});

	return module;
}
