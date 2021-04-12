#include "pppch.h"
#include "Renderer2D.h"

#include "platform/OpenGL/OpenGLShader.h"
#include "pupil/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Pupil {

	struct QuadVertex {
		glm::vec3 Pos;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
		// TODO:: texID
	};

	struct Renderer2DStorage {
		enum Max {
			MaxQuads = 10000,
			MaxVertexs = MaxQuads * 4,
			MaxIndexs = MaxQuads * 6,
			MaxTextures = 32
		};
		glm::vec4 VertexPosition[4];

		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;

		Ref<Shader> TextureShader;

		uint32_t IndicesCount = 0;
		QuadVertex VertexBufferBase[MaxVertexs];
		QuadVertex* VertexBufferptr;

		uint32_t TextureIndex = 1; // index = 0 is whiteTexture 
		Ref<Texture2D> TextureSlots[MaxTextures];

		// Stats
		Renderer2D::Statistics Stats;
	};

	static Renderer2DStorage s_Data;
	
	void Renderer2D::Init() {
		PP_PROFILE_FUNCTION();

		// ------------------------------------------------------------------
		// VAO
		s_Data.VertexArray = VertexArray::Create();

		// VBO + VertexAttribPointer
		s_Data.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertexs * sizeof(QuadVertex));
		s_Data.VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aPos"          },
			{ ShaderDataType::Float4, "aColor"        },
			{ ShaderDataType::Float2, "aTexCoord"     },
			{ ShaderDataType::Float , "aTexIndex"     },
			{ ShaderDataType::Float , "aTilingFactor" }
		});
		s_Data.VertexArray->AddVertexBuffer(s_Data.VertexBuffer);

		// IBO
		uint32_t* indices = new uint32_t[s_Data.MaxIndexs];
		uint32_t offset = 0;
		for (int i = 0; i != s_Data.MaxIndexs; i += 6) {
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			
			indices[i + 3] = offset + 0;
			indices[i + 4] = offset + 2;
			indices[i + 5] = offset + 3;
			offset += 4;
		}
		Ref<IndexBuffer> quadIBO = IndexBuffer::Create(indices, s_Data.MaxIndexs);
		s_Data.VertexArray->SetIndexBuffer(quadIBO);
		delete[] indices;

		// Texture Shader
		int samplers[s_Data.MaxTextures];
		for (int i = 0; i != s_Data.MaxTextures; ++i) samplers[i] = i;

		s_Data.TextureShader = Shader::Create("assets/shaders/Texture");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("Textures", samplers, s_Data.MaxTextures);

		// --------------------------------------------------------------------------------------------------
		// Vertex Position
		s_Data.VertexPosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f }; // x y z w
		s_Data.VertexPosition[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.VertexPosition[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.VertexPosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		// white texture
		uint32_t witeTextureData = 0xffffffff;
		s_Data.TextureSlots[0] = Texture2D::Create(1, 1);
		s_Data.TextureSlots[0]->SetData(&witeTextureData, sizeof(uint32_t));
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		PP_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("ProjectionView", camera.GetProjectionView());

		s_Data.IndicesCount = 0;
		s_Data.VertexBufferptr = s_Data.VertexBufferBase;

		s_Data.TextureIndex = 1;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {
		PP_PROFILE_FUNCTION();

		glm::mat4 projectionViewModel = camera.GetProjection() * glm::mat4(1.0f) * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("ProjectionView", projectionViewModel);

		s_Data.IndicesCount = 0;
		s_Data.VertexBufferptr = s_Data.VertexBufferBase;

		s_Data.TextureIndex = 1;
	}

	void Renderer2D::BeginScene(const EditorCamera& camera) {

		glm::mat4 projectionView = camera.GetProjectionView();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("ProjectionView", projectionView);

		s_Data.IndicesCount = 0;
		s_Data.VertexBufferptr = s_Data.VertexBufferBase;

		s_Data.TextureIndex = 1;
	}

	void Renderer2D::EndScene() {
		PP_PROFILE_FUNCTION();

		uint32_t size = (s_Data.VertexBufferptr - s_Data.VertexBufferBase) * sizeof(QuadVertex);
		s_Data.VertexBuffer->SetData(s_Data.VertexBufferBase, size);

		Flush();
	}

	void Renderer2D::Flush() {
		PP_PROFILE_FUNCTION();

		for (uint32_t i = 0; i != s_Data.TextureIndex; ++i) s_Data.TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(s_Data.VertexArray, s_Data.IndicesCount);

		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset() {
		EndScene();

		s_Data.IndicesCount = 0;
		s_Data.VertexBufferptr = s_Data.VertexBufferBase;

		s_Data.TextureIndex = 1;
	}

	/// Draw Flat ///
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		PP_PROFILE_FUNCTION();

		if (s_Data.IndicesCount >= s_Data.MaxIndexs) {
			PP_CORE_INFO("IndicesCount > MaxIndexs, Reset Batch");
			FlushAndReset();
		}

		constexpr glm::vec2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f } , { 0.0f, 1.0f } };

		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
						* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		const int texIndex = 0; // whiteTexture
		const float tilingFactor = 1.0f;

		constexpr uint32_t quadVertexCount = 4;
		for (uint32_t i = 0; i != quadVertexCount; ++i) {
			s_Data.VertexBufferptr->Pos = model * s_Data.VertexPosition[i];
			s_Data.VertexBufferptr->Color = color;
			s_Data.VertexBufferptr->TexCoord = texCoords[i];
			s_Data.VertexBufferptr->TexIndex = texIndex;
			s_Data.VertexBufferptr->TilingFactor = tilingFactor;
			s_Data.VertexBufferptr++;
		}

		s_Data.IndicesCount += 6;

		s_Data.Stats.QuadCounts++;
	}

	/// Draw Texture ///
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor) {
		PP_PROFILE_FUNCTION();

		if (s_Data.IndicesCount >= s_Data.MaxIndexs) {
			PP_CORE_INFO("IndicesCount > MaxIndexs, Reset Batch");
			FlushAndReset();
		}

		constexpr glm::vec2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f } , { 0.0f, 1.0f } };

		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
						* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		float texIndex = 0.0f;
		for (int i = 0; i != s_Data.TextureIndex; ++i) {
			if (s_Data.TextureSlots[i] == texture) {
				texIndex = (float)i;
				break;
			}
		}
		if (texIndex == 0.0f) {
			s_Data.TextureSlots[s_Data.TextureIndex] = texture;
			texIndex = (float)s_Data.TextureIndex;
			++s_Data.TextureIndex;
		}

		constexpr uint32_t quadVertexCount = 4;
		for (uint32_t i = 0; i != quadVertexCount; ++i) {
			s_Data.VertexBufferptr->Pos = model * s_Data.VertexPosition[i];
			s_Data.VertexBufferptr->Color = tintColor;
			s_Data.VertexBufferptr->TexCoord = texCoords[i];
			s_Data.VertexBufferptr->TexIndex = texIndex;
			s_Data.VertexBufferptr->TilingFactor = tilingFactor;
			s_Data.VertexBufferptr++;
		}

		s_Data.IndicesCount += 6;

		s_Data.Stats.QuadCounts++;
	}

	/// Draw SubTexture2D  ///
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& SubTexture, float tilingFactor, const glm::vec4& tintColor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, SubTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& SubTexture, float tilingFactor, const glm::vec4& tintColor) {
		PP_PROFILE_FUNCTION();

		if (s_Data.IndicesCount >= s_Data.MaxIndexs) {
			PP_CORE_INFO("IndicesCount > MaxIndexs, Reset Batch");
			FlushAndReset();
		}

		const Ref<Texture2D> texture = SubTexture->GetTexture2D();
		const glm::vec2* texCoords = SubTexture->GetTextureCoords();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		float texIndex = 0.0f;
		for (int i = 0; i != s_Data.TextureIndex; ++i) {
			if (s_Data.TextureSlots[i] == texture) {
				texIndex = (float)i;
				break;
			}
		}
		if (texIndex == 0.0f) {
			s_Data.TextureSlots[s_Data.TextureIndex] = texture;
			texIndex = (float)s_Data.TextureIndex;
			++s_Data.TextureIndex;
		}

		constexpr uint32_t quadVertexCount = 4;
		for (uint32_t i = 0; i != quadVertexCount; ++i) {
			s_Data.VertexBufferptr->Pos = model * s_Data.VertexPosition[i];
			s_Data.VertexBufferptr->Color = tintColor;
			s_Data.VertexBufferptr->TexCoord = texCoords[i];
			s_Data.VertexBufferptr->TexIndex = texIndex;
			s_Data.VertexBufferptr->TilingFactor = tilingFactor;
			s_Data.VertexBufferptr++;
		}

		s_Data.IndicesCount += 6;

		s_Data.Stats.QuadCounts++;
	}

	/// Draw Flat ///
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color) {
		if (s_Data.IndicesCount >= s_Data.MaxIndexs) {
			PP_CORE_INFO("IndicesCount > MaxIndexs, Reset Batch");
			FlushAndReset();
		}

		constexpr glm::vec2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f } , { 0.0f, 1.0f } };

		glm::mat4 model = transform;

		const int texIndex = 0; // whiteTexture
		const float tilingFactor = 1.0f;

		constexpr uint32_t quadVertexCount = 4;
		for (uint32_t i = 0; i != quadVertexCount; ++i) {
			s_Data.VertexBufferptr->Pos = model * s_Data.VertexPosition[i];
			s_Data.VertexBufferptr->Color = color;
			s_Data.VertexBufferptr->TexCoord = texCoords[i];
			s_Data.VertexBufferptr->TexIndex = texIndex;
			s_Data.VertexBufferptr->TilingFactor = tilingFactor;
			s_Data.VertexBufferptr++;
		}

		s_Data.IndicesCount += 6;

		s_Data.Stats.QuadCounts++;
	}

	/// Draw Rotate Flat ///
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		PP_PROFILE_FUNCTION();
		
		if (s_Data.IndicesCount >= s_Data.MaxIndexs) {
			PP_CORE_INFO("IndicesCount > MaxIndexs, Reset Batch");
			FlushAndReset();
		}

		constexpr glm::vec2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f } , { 0.0f, 1.0f } };

		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
						* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
						* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		const int texIndex = 0; // whiteTexture
		const float tilingFactor = 1.0f;

		constexpr uint32_t quadVertexCount = 4;
		for (uint32_t i = 0; i != quadVertexCount; ++i) {
			s_Data.VertexBufferptr->Pos = model * s_Data.VertexPosition[i];
			s_Data.VertexBufferptr->Color = color;
			s_Data.VertexBufferptr->TexCoord = texCoords[i];
			s_Data.VertexBufferptr->TexIndex = texIndex;
			s_Data.VertexBufferptr->TilingFactor = tilingFactor;
			s_Data.VertexBufferptr++;
		}

		s_Data.IndicesCount += 6;

		s_Data.Stats.QuadCounts++;
	}

	///  Draw Rotate Texture ///
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor /*= 1*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/) {
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor /*= 1*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/) {
		PP_PROFILE_FUNCTION();

		if (s_Data.IndicesCount >= s_Data.MaxIndexs) {
			PP_CORE_INFO("IndicesCount > MaxIndexs, Reset Batch");
			FlushAndReset();
		}

		constexpr glm::vec2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f } , { 0.0f, 1.0f } };

		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
						* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
						* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		float texIndex = 0.0f;
		for (int i = 0; i != s_Data.TextureIndex; ++i) {
			if (s_Data.TextureSlots[i] == texture) {
				texIndex = (float)i;
				break;
			}
		}
		if (texIndex == 0.0f) {
			s_Data.TextureSlots[s_Data.TextureIndex] = texture;
			texIndex = (float)s_Data.TextureIndex;
			++s_Data.TextureIndex;
		}

		constexpr uint32_t quadVertexCount = 4;
		for (uint32_t i = 0; i != quadVertexCount; ++i) {
			s_Data.VertexBufferptr->Pos = model * s_Data.VertexPosition[i];
			s_Data.VertexBufferptr->Color = tintColor;
			s_Data.VertexBufferptr->TexCoord = texCoords[i];
			s_Data.VertexBufferptr->TexIndex = texIndex;
			s_Data.VertexBufferptr->TilingFactor = tilingFactor;
			s_Data.VertexBufferptr++;
		}

		s_Data.IndicesCount += 6;

		s_Data.Stats.QuadCounts++;
	}

	///  Draw Rotate SubTexture ///
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& SubTexture, float tilingFactor /*= 1*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/) {
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, SubTexture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& SubTexture, float tilingFactor /*= 1*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/) {
		PP_PROFILE_FUNCTION();

		if (s_Data.IndicesCount >= s_Data.MaxIndexs) {
			PP_CORE_INFO("IndicesCount > MaxIndexs, Reset Batch");
			FlushAndReset();
		}

		const Ref<Texture2D> texture = SubTexture->GetTexture2D();
		const glm::vec2* texCoords = SubTexture->GetTextureCoords();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		float texIndex = 0.0f;
		for (int i = 0; i != s_Data.TextureIndex; ++i) {
			if (s_Data.TextureSlots[i] == texture) {
				texIndex = (float)i;
				break;
			}
		}
		if (texIndex == 0.0f) {
			s_Data.TextureSlots[s_Data.TextureIndex] = texture;
			texIndex = (float)s_Data.TextureIndex;
			++s_Data.TextureIndex;
		}

		constexpr uint32_t quadVertexCount = 4;
		for (uint32_t i = 0; i != quadVertexCount; ++i) {
			s_Data.VertexBufferptr->Pos = model * s_Data.VertexPosition[i];
			s_Data.VertexBufferptr->Color = tintColor;
			s_Data.VertexBufferptr->TexCoord = texCoords[i];
			s_Data.VertexBufferptr->TexIndex = texIndex;
			s_Data.VertexBufferptr->TilingFactor = tilingFactor;
			s_Data.VertexBufferptr++;
		}

		s_Data.IndicesCount += 6;

		s_Data.Stats.QuadCounts++;
	}

	void Renderer2D::ResetStats() {
		memset(&s_Data.Stats, 0, sizeof(s_Data.Stats));
	}

	Renderer2D::Statistics Renderer2D::GetStats() {
		return s_Data.Stats;
	}

}