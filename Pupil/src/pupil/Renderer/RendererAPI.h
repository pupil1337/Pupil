#pragma once
#include "pupil/Core/Core.h"

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
		virtual ~RendererAPI() = default;
		virtual void Init() const = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const = 0;
		virtual void SetClearColor(const glm::vec4& color) const = 0;
		virtual void Clear() const = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) const = 0;

		static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

}