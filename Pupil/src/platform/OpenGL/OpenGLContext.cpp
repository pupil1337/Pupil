#include "pppch.h"
#include "OpenGLContext.h"

#include "pupil/Application.h"

namespace Pupil {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle) {
		PP_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init() override {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PP_CORE_ASSERT(status, "Failed to initialized Glad!");
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}

}