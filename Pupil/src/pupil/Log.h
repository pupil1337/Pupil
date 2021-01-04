#pragma once

#include"Core.h"
#include"spdlog/spdlog.h"
#include"spdlog/fmt/ostr.h"
#include"spdlog/sinks/stdout_color_sinks.h"

namespace Pupil {

	class PP_API Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}


// Core log macros

#define PP_CORE_TRACE(...)  ::Pupil::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PP_CORE_INFO(...)   ::Pupil::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PP_CORE_WARN(...)   ::Pupil::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PP_CORE_ERROR(...)  ::Pupil::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PP_CORE_CRITICAL(...)  ::Pupil::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define PP_TRACE(...)		::Pupil::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PP_INFO(...)		::Pupil::Log::GetClientLogger()->info(__VA_ARGS__)
#define PP_WARN(...)		::Pupil::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PP_ERROR(...)		::Pupil::Log::GetClientLogger()->error(__VA_ARGS__)
#define PP_CRITICAL(...)	::Pupil::Log::GetClientLogger()->critical(__VA_ARGS__)

// if dist build
//#define PP_CORE_INFO


