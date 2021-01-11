#include "pppch.h"
#include "renderer.h"

namespace Pupil {
	//#ifdef PP_OpenGL_API
		RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;
	//#else
		//#ifdef PP_Direct3D_API
			//RendererAPI Renderer::s_RendererAPI = RendererAPI::Direct3D;
		//#endif
	//#endif
}