#include "pppch.h"
#include "Framebuffer.h"

#include "renderer.h"
#include "platform/OpenGL/OpenGLFramebuffer.h"

namespace Pupil {

	Pupil::Ref<Pupil::Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:     PP_CORE_ASSERT(false, "RenderAPI:None is supported!"); return nullptr;
			case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLFramebuffer>(spec);
			case RendererAPI::API::Direct3D: PP_CORE_ASSERT(false, "RenderAPI:Didn't write Dirext3D-API code!"); return nullptr;
			case RendererAPI::API::Vulcan:   PP_CORE_ASSERT(false, "RenderAPI:Didn't write Vulcan-API code!"); return nullptr;
		}
		PP_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

}