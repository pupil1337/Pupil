#include "pppch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "platform/OpenGL/OpenGLBuffer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"

namespace Pupil {

	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
			case RendererAPI::None:     PP_CORE_ASSERT(false, "RenderAPI:None is supported!"); return nullptr;
			case RendererAPI::OpenGL:   return new OpenGLVertexArray();
			case RendererAPI::Direct3D: PP_CORE_ASSERT(false, "RenderAPI:Didn't write Dirext3D-API code!"); return nullptr;
			case RendererAPI::Vulcan:   PP_CORE_ASSERT(false, "RenderAPI:Didn't write Vulcan-API code!"); return nullptr;
		}
		PP_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

}