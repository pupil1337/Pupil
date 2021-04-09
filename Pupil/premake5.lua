project "Pupil"
	kind "Staticlib"
	language "C++"
	cppdialect"C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pppch.h"
	pchsource "src/pppch.cpp"

	files {
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs {
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Entt}",
		"%{IncludeDir.yaml_cpp}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
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
