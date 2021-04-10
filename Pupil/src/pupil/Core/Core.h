#pragma once

#include <memory>

#ifdef _WIN32
	#ifdef _WIN64
		#define PP_PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define PP_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define PP_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknow Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define PP_PLATFORM_ANDROID
	#error "Android is not supported"
#elif defined(__LINUX__)
	#define PP_PLATFORM_LINUX
	#error "Linux is not supported"
#else
	#error "Unknow platform!"
#endif


#ifdef PP_PLATFORM_WINDOWS
	#if PP_DYNAMIC_LINK
		#ifdef PP_BUILD_DLL
			#define PP_API __declspec(dllexport)
		#else
			#define PP_API __declspec(dllimport)
		#endif
	#else
		#define PP_API
	#endif
#else
	#error Pupil only supports Windows!
#endif

#ifdef PP_DEBUG
	#define PP_ENABLE_ASSERTS
#endif

#include "Log.h"
#ifdef PP_ENABLE_ASSERTS
	#define PP_ASSERT(x, ...) { if(!(x)) { PP_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define PP_CORE_ASSERT(x, ...) { if(!(x)) { PP_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define PP_ASSERT(x, ...)
	#define PP_CORE_ASSERT(x, ...)
#endif


/////////////////////////////////////////////////////////////////////////////////////////////
/// Myself define ///////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


#define BIT(x) (1 << x)

#define PP_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
//#define PP_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Pupil {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}