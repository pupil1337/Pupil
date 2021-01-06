#pragma once

#ifdef PP_PLATFORM_WINDOWS
	#ifdef PP_BUILD_DLL
		#define PP_API __declspec(dllexport)
	#else
		#define PP_API __declspec(dllimport)
	#endif
#else
	#error Pupil only supports Windows!
#endif

#ifdef PP_ENABLE_ASSERTS
	#define PP_ASSERT(x, ...) { if(!x) { PP_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define PP_CORE_ASSERT(x, ...) { if(!x) { PP_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define PP_ASSERT(x, ...)
	#define PP_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define PP_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)