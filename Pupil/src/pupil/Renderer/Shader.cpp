#include "pppch.h"
#include "Shader.h"

#include "pupil/Renderer/Renderer.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace Pupil {

	Ref<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:     PP_CORE_ASSERT(false, "RenderAPI:None is supported!"); return nullptr;
			case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLShader>(vertexPath, fragmentPath, geometryPath);
			case RendererAPI::API::Direct3D: PP_CORE_ASSERT(false, "RenderAPI:Didn't write Dirext3D-API code!"); return nullptr;
			case RendererAPI::API::Vulcan:   PP_CORE_ASSERT(false, "RenderAPI:Didn't write Vulcan-API code!"); return nullptr;
		}
		PP_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

}