#include "pppch.h"
#include "Renderer2D.h"

#include "platform/OpenGL/OpenGLShader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Pupil {

	struct QuadVertex {
		glm::vec3 Pos;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		// TODO:: color, texID
	};

	struct Renderer2DStorage {
		enum Max {
			MaxQuads = 10000,
			MaxVertexs = MaxQuads * 4,
			MaxIndexs = MaxQuads * 6
		};

		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;

		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		QuadVertex VertexBufferBase[MaxVertexs];
		QuadVertex* VertexBufferptr;
		uint32_t IndicesCount;
	};

	static Renderer2DStorage s_Data;
	
	void Renderer2D::Init() {
		PP_PROFILE_FUNCTION();

		// ------------------------------------------------------------------
		// VAO
		s_Data.VertexArray = Pupil::VertexArray::Create();

		// VBO + VertexAttribPointer
		s_Data.VertexBuffer = Pupil::VertexBuffer::Create(s_Data.MaxVertexs * sizeof(QuadVertex));
		s_Data.VertexBuffer->SetLayout({
			{ Pupil::ShaderDataType::Float3, "aPos" },
			{ Pupil::ShaderDataType::Float4, "aColor" },
			{ Pupil::ShaderDataType::Float2, "aTexCoord" }
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
		Pupil::Ref<Pupil::IndexBuffer> quadIBO = Pupil::IndexBuffer::Create(indices, s_Data.MaxIndexs);
		s_Data.VertexArray->SetIndexBuffer(quadIBO);
		delete[] indices;
		// ------------------------------------------------------------------

		s_Data.TextureShader = Shader::Create("assets/shaders/Texture");
		// s_Data.m_TextureShader->Bind();
		// s_Data.m_TextureShader->SetInt("Texture0", 0);

		// white texture
		uint32_t witeTextureData = 0xffffffff;
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		s_Data.WhiteTexture->SetData(&witeTextureData, sizeof(uint32_t));
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		PP_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("ProjectionView", camera.GetProjectionView());

		s_Data.IndicesCount = 0;
		s_Data.VertexBufferptr = s_Data.VertexBufferBase;
	}

	void Renderer2D::EndScene() {
		PP_PROFILE_FUNCTION();

		uint32_t size = (s_Data.VertexBufferptr - s_Data.VertexBufferBase) * sizeof(QuadVertex);
		s_Data.VertexBuffer->SetData(s_Data.VertexBufferBase, size);

		Flush();
	}

	void Renderer2D::Flush() {
		PP_PROFILE_FUNCTION();

		RenderCommand::DrawIndexed(s_Data.VertexArray, s_Data.IndicesCount);
	}

	/// Draw Flat ///
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		PP_PROFILE_FUNCTION();

		s_Data.VertexBufferptr->Pos = { position.x, position.y, position.z };
		s_Data.VertexBufferptr->Color = color;
		s_Data.VertexBufferptr->TexCoord = { 0.0f, 0.0f };
		s_Data.VertexBufferptr++;

		s_Data.VertexBufferptr->Pos = { position.x, position.y + size.y, position.z };
		s_Data.VertexBufferptr->Color = color;
		s_Data.VertexBufferptr->TexCoord = { 0.0f, 1.0f };
		s_Data.VertexBufferptr++;

		s_Data.VertexBufferptr->Pos = { position.x + size.x, position.y + size.y, position.z };
		s_Data.VertexBufferptr->Color = color;
		s_Data.VertexBufferptr->TexCoord = { 1.0f, 1.0f };
		s_Data.VertexBufferptr++;

		s_Data.VertexBufferptr->Pos = { position.x + size.x, position.y, position.z };
		s_Data.VertexBufferptr->Color = color;
		s_Data.VertexBufferptr->TexCoord = { 1.0f, 0.0f };
		s_Data.VertexBufferptr++;

		s_Data.IndicesCount += 6;

		/*s_Data.m_TextureShader->Bind();
		s_Data.m_TextureShader->SetVec4("Color", color);
		s_Data.m_WhiteTexture->Bind(0);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.m_TextureShader->SetMat4("Model", model);
		RenderCommand::DrawIndexed(s_Data.m_VertexArray);*/
	}

	/// Draw Texture ///
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float filingFactor, const glm::vec4& tintColor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, filingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float filingFactor, const glm::vec4& tintColor) {
		PP_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetVec4("Color", tintColor);
		s_Data.TextureShader->SetFloat("FilingFactor", filingFactor);
		texture->Bind(0);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("Model", model);
		RenderCommand::DrawIndexed(s_Data.VertexArray);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		PP_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetVec4("Color", color);
		s_Data.WhiteTexture->Bind(0);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("Model", model);
		RenderCommand::DrawIndexed(s_Data.VertexArray);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float filingFactor /*= 1*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/) {
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, texture, filingFactor, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float filingFactor /*= 1*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/) {
		PP_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetVec4("Color", tintColor);
		s_Data.TextureShader->SetFloat("FilingFactor", filingFactor);
		texture->Bind(0);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("Model", model);
		RenderCommand::DrawIndexed(s_Data.VertexArray);
	}

}