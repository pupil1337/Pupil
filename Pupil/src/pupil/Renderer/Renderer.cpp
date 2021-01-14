#include "pppch.h"
#include "renderer.h"

namespace Pupil {

	void Renderer::BeginScene() {
	}

	void Renderer::EndScene() {
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) {
		RenderCommand::DrawIndexed(vertexArray);
	}

}