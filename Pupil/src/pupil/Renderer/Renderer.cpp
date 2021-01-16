#include "pppch.h"
#include "renderer.h"

namespace Pupil {

	Renderer::ScenceData* Renderer::m_ScenceData = new Renderer::ScenceData;

	void Renderer::BeginScene(const OrthographicCamera& camera) {
		m_ScenceData->ProjectionView = camera.GetProjectionView();
	}

	void Renderer::EndScene() {
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& model) {
		shader->Bind();
		shader->SetMat4("ProjectonView", m_ScenceData->ProjectionView);
		shader->SetMat4("Model", model);
		RenderCommand::DrawIndexed(vertexArray);
		shader->UnBind();
	}

}