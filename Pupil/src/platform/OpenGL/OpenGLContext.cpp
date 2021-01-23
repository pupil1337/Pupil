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

		#ifdef PP_ENABLE_ASSERTS
			int versionMajor;
			int versionMinor;
			glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
			glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
			PP_CORE_ASSERT(versionMajor > 4 || versionMajor == 4 && versionMinor >= 5, "Pupil requires at least OpenGL Version: 4.5!");
		#endif
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}

}