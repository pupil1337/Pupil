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
IncludeDir["stb_image"] = "Pupil/vendor/stb_image"

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
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
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
			optimize "on"

		filter "configurations:Dist"
			defines "PP_Dist"
			runtime "Release"
			optimize "on"

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
		systemversion "latest"

		defines {
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
			

project "Pupil-Editor"
	location "Pupil-Editor"
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
		systemversion "latest"

		defines {
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
