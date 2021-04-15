#pragma once
#include "pupil/Core/Core.h"

#include "pupil/Renderer/RendererAPI.h"
#include "pupil/Renderer/Shader.h"
#include "pupil/Renderer/VertexArray.h"
#include "pupil/Renderer/OrthographicCamera.h"

#include "pupil/Renderer/Texture.h"
#include "pupil/Renderer/SubTexture2D.h"

#include "pupil/Renderer/Camera.h"
#include "pupil/Renderer/EditorCamera.h"

namespace Pupil {

	class PP_API Renderer2D {
	public:
		static void Init();

		static void BeginScene(const OrthographicCamera& camera);// ToDo remove
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void Flush();

		static void DrawQuad(int entityID, const glm::mat4& transform, const glm::vec4& color);
		static void DrawLinear(const glm::mat4& transform, const EditorCamera& camera);

		// ToDo¡ý¡ý¡ý Delete
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		// todo     DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture)
		// todo AndSoON...
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1, const glm::vec4& tintColor = glm::vec4(1.0f));
		// ToDo¡ü¡ü¡ü Delete
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