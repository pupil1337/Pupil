#include "pppch.h"
#include "renderer.h"

#include "platform/OpenGL/OpenGLShader.h"
#include "Renderer2D.h"

namespace Pupil {

	Scope<Renderer::ScenceData> Renderer::m_ScenceData = std::make_unique<Renderer::ScenceData>();

	void Renderer::Init() {
		PP_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::BeginScene(const OrthographicCamera& camera) {
		PP_PROFILE_FUNCTION();

		m_ScenceData->ProjectionView = camera.GetProjectionView();
	}

	void Renderer::EndScene() {
		PP_PROFILE_FUNCTION();

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& model) {
		PP_PROFILE_FUNCTION();

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetMat4("ProjectionView", m_ScenceData->ProjectionView);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetMat4("Model", model);
		RenderCommand::DrawIndexed(vertexArray);
		shader->UnBind();
	}

}