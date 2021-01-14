#pragma once
#include "pupil/Core.h"

#include "pupil/Renderer/RendererAPI.h"

namespace Pupil {

	class PP_API OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void SetClearColor(const glm::vec4& color) const override;
		virtual void Clear() const override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) const override;
	};
}