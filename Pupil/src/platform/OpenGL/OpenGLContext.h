#pragma once

#include "pupil/Renderer/GraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Pupil {

	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};

}