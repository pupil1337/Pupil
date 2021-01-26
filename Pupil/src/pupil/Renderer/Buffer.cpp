#include "pppch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "platform/OpenGL/OpenGLBuffer.h"

namespace Pupil {

	// *****VertexBuffer***** //
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
		PP_PROFILE_FUNCTION();

		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:     PP_CORE_ASSERT(false, "RenderAPI:None is supported!"); return nullptr;
		case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLVertexBuffer>(size);
		case RendererAPI::API::Direct3D: PP_CORE_ASSERT(false, "RenderAPI:Didn't write Dirext3D-API code!"); return nullptr;
		case RendererAPI::API::Vulcan:   PP_CORE_ASSERT(false, "RenderAPI:Didn't write Vulcan-API code!"); return nullptr;
		}
		PP_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		PP_PROFILE_FUNCTION();

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:     PP_CORE_ASSERT(false, "RenderAPI:None is supported!"); return nullptr;
			case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLVertexBuffer>(vertices, size);
			case RendererAPI::API::Direct3D: PP_CORE_ASSERT(false, "RenderAPI:Didn't write Dirext3D-API code!"); return nullptr;
			case RendererAPI::API::Vulcan:   PP_CORE_ASSERT(false, "RenderAPI:Didn't write Vulcan-API code!"); return nullptr;
		}
		PP_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}


	// *****IndexBuffer***** //
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		PP_PROFILE_FUNCTION();

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:     PP_CORE_ASSERT(false, "RenderAPI:None is supported!"); return nullptr;
			case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLIndexBuffer>(indices, count);
			case RendererAPI::API::Direct3D: PP_CORE_ASSERT(false, "RenderAPI:Didn't write Dirext3D-API code!"); return nullptr;
			case RendererAPI::API::Vulcan:   PP_CORE_ASSERT(false, "RenderAPI:Didn't write Vulcan-API code!"); return nullptr;
		}
		PP_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

}