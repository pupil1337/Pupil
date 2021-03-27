#include "pppch.h"

#include "pupil/Core/Application.h"
#include <GLFW/glfw3.h>

#include "pupil/Core/Input.h"

namespace Pupil {

	bool Input::IsKeyPressed(int keycode) {
		PP_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMousePressed(int button) {
		PP_PROFILE_FUNCTION();

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition() {
		PP_PROFILE_FUNCTION();    

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX() {
		PP_PROFILE_FUNCTION();

		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY() {
		PP_PROFILE_FUNCTION();

		auto [x, y] = GetMousePosition();
		return y;
	}

}