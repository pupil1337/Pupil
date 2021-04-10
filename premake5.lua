workspace "Pupil"
	architecture "x64"
	startproject "Pupil-Editor"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"]      = "%{wks.location}/Pupil/vendor/GLFW/include"
IncludeDir["Glad"]      = "%{wks.location}/Pupil/vendor/Glad/include"
IncludeDir["ImGui"]     = "%{wks.location}/Pupil/vendor/imgui"
IncludeDir["glm"]       = "%{wks.location}/Pupil/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Pupil/vendor/stb_image"
IncludeDir["Entt"]      = "%{wks.location}/Pupil/vendor/Entt/include"
IncludeDir["yaml_cpp"]  = "%{wks.location}/Pupil/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"]  = "%{wks.location}/Pupil/vendor/ImGuizmo"

group "Dependencies"
	include "Pupil/vendor/GLFW"
	include "Pupil/vendor/Glad"
	include "Pupil/vendor/imgui"
	include "Pupil/vendor/yaml-cpp"
group ""

include "Pupil"
include "Pupil-Editor"
include "Sandbox"
