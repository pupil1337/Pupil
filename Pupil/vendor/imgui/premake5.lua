project "ImGui"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"*.h",
		"*.cpp"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "10.0.18362.0"
		staticruntime "On"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
