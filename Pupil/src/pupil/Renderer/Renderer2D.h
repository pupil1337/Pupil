#pragma once
#include "pupil/Core/Core.h"

#include "RendererAPI.h"
#include "Shader.h"
#include "VertexArray.h"
#include "OrthographicCamera.h"

#include "Texture.h"

namespace Pupil {

	class PP_API Renderer2D {
	public:
		static void Init();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);

	private:
		struct Renderer2DStorage {
			Ref<VertexArray> m_VertexArray;
			Ref<Shader> m_SimpleShader;
			Ref<Shader> m_TextureShader;
		};

		static Scope<Renderer2DStorage> m_Data;
	};
}