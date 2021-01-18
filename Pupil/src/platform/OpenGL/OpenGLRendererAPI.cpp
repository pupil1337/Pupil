#include "pppch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Pupil {

	void OpenGLRendererAPI::Init() const {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() const {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) const {
		vertexArray->Bind();
		vertexArray->GetIndexBuffer()->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		vertexArray->GetIndexBuffer()->UnBind();
		vertexArray->UnBind();
	}

}