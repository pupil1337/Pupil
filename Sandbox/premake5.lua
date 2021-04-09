project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect"C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"%{wks.location}/Pupil/vendor/spdlog/include",
		"%{wks.location}/Pupil/src",
		"%{wks.location}/Pupil/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Entt}"
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
