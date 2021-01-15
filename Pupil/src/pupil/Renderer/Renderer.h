#pragma once
#include "pupil/Core.h"

#include "pupil/Renderer/RendererAPI.h"
#include "pupil/Renderer/RenderCommand.h"

#include "pupil/Renderer/Shader.h"
#include "pupil/Renderer/OrthographicCamera.h"

namespace Pupil {

	class PP_API Renderer {
	public:
		static void BeginScene(std::shared_ptr<OrthographicCamera>& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	
	private:
		struct ScenceData {
			glm::mat4 ProjectionView;
		};

		static ScenceData* m_ScenceData;
	};

}