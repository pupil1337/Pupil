#include "pppch.h"
#include "OpenGLContext.h"

#include "pupil/Core/Application.h"

namespace Pupil {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle) {
		PP_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PP_CORE_ASSERT(status, "Failed to initialized Glad!");

		PP_CORE_INFO("OpenGL Info:");
		PP_CORE_INFO("  Vendor:   {0}", glGetString(GL_VENDOR));
		PP_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		PP_CORE_INFO("  Version:  {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}

}