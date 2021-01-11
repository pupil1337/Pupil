#pragma once

#include "pupil/Core.h"

namespace Pupil {

	enum class RendererAPI {
		None     = 0,
		OpenGL   = 1,
		Direct3D = 2,
		Vulcan	 = 3
	};

	class PP_API Renderer {
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};

}