#include "pppch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "platform/OpenGL/OpenGLBuffer.h"

namespace Pupil {

	// *****VertexBuffer***** //
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:     PP_CORE_ASSERT(false, "RenderAPI:None is supported!"); return nullptr;
			case RendererAPI::API::OpenGL:   return new OpenGLVertexBuffer(vertices, size);
			case RendererAPI::API::Direct3D: PP_CORE_ASSERT(false, "RenderAPI:Didn't write Dirext3D-API code!"); return nullptr;
			case RendererAPI::API::Vulcan:   PP_CORE_ASSERT(false, "RenderAPI:Didn't write Vulcan-API code!"); return nullptr;
		}
		PP_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

	// *****IndexBuffer***** //
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:     PP_CORE_ASSERT(false, "RenderAPI:None is supported!"); return nullptr;
			case RendererAPI::API::OpenGL:   return new OpenGLIndexBuffer(indices, count);
			case RendererAPI::API::Direct3D: PP_CORE_ASSERT(false, "RenderAPI:Didn't write Dirext3D-API code!"); return nullptr;
			case RendererAPI::API::Vulcan:   PP_CORE_ASSERT(false, "RenderAPI:Didn't write Vulcan-API code!"); return nullptr;
		}
		PP_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

}