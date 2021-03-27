#pragma once

#include "Core.h"

namespace Pupil {

	class PP_API Input {

	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMousePressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	};

}