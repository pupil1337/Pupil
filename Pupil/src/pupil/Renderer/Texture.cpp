#include "pppch.h"
#include "Texture.h"

#include "pupil/Renderer/Renderer.h"
#include "platform/OpenGL/OpenGLTexture.h"

namespace Pupil {

	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:     PP_CORE_ASSERT(false, "RenderAPI:None is supported!"); return nullptr;
			case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLTexture2D>(path);
			case RendererAPI::API::Direct3D: PP_CORE_ASSERT(false, "RenderAPI:Didn't write Dirext3D-API code!"); return nullptr;
			case RendererAPI::API::Vulcan:   PP_CORE_ASSERT(false, "RenderAPI:Didn't write Vulcan-API code!"); return nullptr;
		}
		PP_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}
}