#pragma once
#include "pupil/Core.h"

#include "pupil/Renderer/RendererAPI.h"
#include "pupil/Renderer/RenderCommand.h"

#include "pupil/Renderer/Shader.h"
#include "pupil/Renderer/OrthographicCamera.h"

namespace Pupil {

	class PP_API Renderer {
	public:
		static void Init();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& model = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	
	private:
		struct ScenceData {
			glm::mat4 ProjectionView;
		};

		static ScenceData* m_ScenceData;
	};

}