#pragma once
#include "pupil/Core.h"

#include "glm/glm.hpp"

#include <memory>
#include "VertexArray.h"

namespace Pupil {

	class PP_API RendererAPI {
	public:
		enum class API {
			None = 0,
			OpenGL = 1,
			Direct3D = 2,
			Vulcan = 3
		};
	public:
		virtual void SetClearColor(const glm::vec4& color) const = 0;
		virtual void Clear() const = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) const = 0;

		static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}