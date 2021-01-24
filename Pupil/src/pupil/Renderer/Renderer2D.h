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
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float filingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float filingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float filingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float filingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));

	private:
		struct Renderer2DStorage {
			Ref<VertexArray> m_VertexArray;
			Ref<Shader> m_TextureShader;
			Ref<Texture2D> m_WhiteTexture;
		};

		static Scope<Renderer2DStorage> m_Data;
	};
}