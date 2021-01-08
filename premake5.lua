workspace "Pupil"
	architecture "x64"
	startproject "Sandbox"

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
IncludeDir["ImGui"] = "Pupil/vendor/imgui"
IncludeDir["glm"] = "Pupil/vendor/glm"

include "Pupil/vendor/GLFW"
include "Pupil/vendor/Glad"
include "Pupil/vendor/imgui"


project "Pupil"
	location "Pupil"
	kind "Staticlib"
	language "C++"
	cppdialect"C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pppch.h"
	pchsource "Pupil/src/pppch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "10.0.18362.0"

		defines {
			"PP_PLATFORM_WINDOWS",
			"PP_BUILD_DLL",
			"PP_DEBUG"
		}

		filter "configurations:Debug"
			defines "PP_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "PP_Release"
			runtime "Release"
			symbols "on"

		filter "configurations:Dist"
			defines "PP_Dist"
			runtime "Release"
			symbols "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect"C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Pupil/vendor/spdlog/include",
		"Pupil/src",
		"Pupil/vendor",
		"%{IncludeDir.glm}"
	}

	links {
		"Pupil",
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "10.0.18362.0"

		defines {
			"PP_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "PP_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "PP_Release"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "PP_Dist"
			runtime "Release"
			optimize "on"
