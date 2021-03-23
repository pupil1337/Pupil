#pragma once
#include "pupil/Core/Core.h"

#include "RendererAPI.h"
#include "Shader.h"
#include "VertexArray.h"
#include "OrthographicCamera.h"

#include "Texture.h"
#include "SubTexture2D.h"

namespace Pupil {

	class PP_API Renderer2D {
	public:
		static void Init();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));

		// Stats
		struct Statistics {
			uint32_t DrawCalls = 0;
			uint32_t QuadCounts = 0;

			uint32_t GetTotalVertexCount() { return QuadCounts * 4; }
			uint32_t GetTotalIndexCount() { return QuadCounts * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();

	private:
		static void FlushAndReset();
	};
}