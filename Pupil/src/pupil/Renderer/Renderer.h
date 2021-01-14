#pragma once
#include "pupil/Core.h"

#include "pupil/Renderer/RendererAPI.h"
#include "pupil/Renderer/RenderCommand.h"

namespace Pupil {

	class PP_API Renderer {
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}