#pragma once
#include "pupil/Core/Core.h"

#include "RendererAPI.h"
#include "Shader.h"
#include "VertexArray.h"
#include "OrthographicCamera.h"

namespace Pupil {

	class PP_API Renderer2D {
	public:
		static void Init();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& model = glm::mat4(1.0f));

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

	private:
		struct Renderer2DStorage {
			Ref<VertexArray> m_VertexArray;
			Ref<Shader> m_Shader;
		};

		static Scope<Renderer2DStorage> m_Data;
	};
}