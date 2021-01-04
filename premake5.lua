workspace "Pupil"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Pupil/vendor/GLFW/include"
IncludeDir["Glad"] = "Pupil/vendor/Glad/include"

include "Pupil/vendor/GLFW"
include "Pupil/vendor/Glad"


project "Pupil"
	location "Pupil"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pppch.h"
	pchsource "Pupil/src/pppch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links {
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.18362.0"

		defines {
			"PP_PLATFORM_WINDOWS",
			"PP_BUILD_DLL",
			"PP_ENABLE_ASSERTS"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

		filter "configurations:Debug"
			defines "PP_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "PP_Release"
			buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "PP_Dist"
			buildoptions "/MD"
			optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Pupil/vendor/spdlog/include",
		"Pupil/src"
	}

	links {
		"Pupil"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0.18362.0"

		defines {
			"PP_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "PP_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "PP_Release"
			buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "PP_Dist"
			buildoptions "/MD"
			optimize "On"
