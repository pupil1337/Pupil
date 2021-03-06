#include "pppch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Pupil {

	void OpenGLRendererAPI::Init() const {
		PP_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0x00);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const {
		PP_PROFILE_FUNCTION();

		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const {
		PP_PROFILE_FUNCTION();

		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() const {
		PP_PROFILE_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count) const {
		PP_PROFILE_FUNCTION();

		vertexArray->Bind();
		uint32_t drawCount = count ? count : vertexArray->GetIndexBuffer()->GetCount();
		vertexArray->GetIndexBuffer()->Bind();
		glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, nullptr);
	}

}