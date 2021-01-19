#include "pppch.h"
#include "Shader.h"

#include "pupil/Renderer/Renderer.h"
#include "platform/OpenGL/OpenGLShader.h"

namespace Pupil {

	Ref<Shader> Shader::Create(const std::string& filePath_WithOutDot) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:     PP_CORE_ASSERT(false, "RenderAPI:None is supported!"); return nullptr;
			case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLShader>(filePath_WithOutDot);
			case RendererAPI::API::Direct3D: PP_CORE_ASSERT(false, "RenderAPI:Didn't write Dirext3D-API code!"); return nullptr;
			case RendererAPI::API::Vulcan:   PP_CORE_ASSERT(false, "RenderAPI:Didn't write Vulcan-API code!"); return nullptr;
		}
		PP_CORE_ASSERT(false, "Unknow RendererAPI!");
		return nullptr;
	}

	//////////////////////////////////////////////////////////////////////////////////
	////// ShaderLibrary /////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////

	bool ShaderLibrary::Exist(const std::string& name) {
		return m_Shaders.find(name) != m_Shaders.end();
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
		PP_CORE_ASSERT(!Exist(name), "Shader is already exist!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Load(const std::string& name, const std::string& filePath_WithOutDot) {
		Ref<Shader> shader = Shader::Create(filePath_WithOutDot);
		Add(name, shader);
	}

	void ShaderLibrary::Load(const std::string& filePath_WithOutDot) {
		Ref<Shader> shader = Shader::Create(filePath_WithOutDot);
		const std::string& name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::GetShader(const std::string& name) {
		PP_CORE_ASSERT(Exist(name), "Shader is not find");
		return m_Shaders[name];
	}

}