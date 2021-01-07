project "ImGui"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"imconfig.h",
        "imgui.cpp",
        "imgui.h",
        "imgui_demo.cpp",
        "imgui_draw.cpp",
        "imgui_internal.h",
        "imgui_widgets.cpp",
        "imgstb_rectpack.h",
        "imgstb_textedit.h",
        "imgstb_truetype.h"
	}

	filter "system:windows"
		systemversion "10.0.18362.0"
        cppdialect "C++17"
		staticruntime "On"


	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"