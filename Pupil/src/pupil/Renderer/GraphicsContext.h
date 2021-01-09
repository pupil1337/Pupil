#pragma once

namespace Pupil {

	class PP_API GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}