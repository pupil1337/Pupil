#include "pppch.h"
#include "Renderer2D.h"

#include "platform/OpenGL/OpenGLShader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Pupil {

	Scope<Renderer2D::Renderer2DStorage> Renderer2D::m_Data = std::make_unique<Renderer2D::Renderer2DStorage>();
	
	void Renderer2D::Init() {
		PP_PROFILE_FUNCTION();

		float vertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f
		};

		m_Data->m_VertexArray = Pupil::VertexArray::Create();

		Pupil::Ref<Pupil::VertexBuffer> vertexBuffer = Pupil::VertexBuffer::Create(vertices, sizeof(vertices));
		Pupil::BufferLayout layout = {
			{ Pupil::ShaderDataType::Float3, "aPos" },
			{ Pupil::ShaderDataType::Float2, "aTexCoord" }
		};
		vertexBuffer->SetLayout(layout);
		m_Data->m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 1, 3, 2 };
		Pupil::Ref<Pupil::IndexBuffer> indexBuffer = Pupil::Ref<Pupil::IndexBuffer>(Pupil::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_Data->m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Data->m_TextureShader = Shader::Create("assets/shaders/Texture");
		m_Data->m_TextureShader->Bind();
		m_Data->m_TextureShader->SetInt("Texture0", 0);

		// white texture
		uint32_t witeTextureData = 0xffffffff;
		m_Data->m_WhiteTexture = Texture2D::Create(1, 1);
		m_Data->m_WhiteTexture->SetData(&witeTextureData, sizeof(uint32_t));
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		PP_PROFILE_FUNCTION();

		m_Data->m_TextureShader->Bind();
		m_Data->m_TextureShader->SetMat4("ProjectionView", camera.GetProjectionView());
	}

	void Renderer2D::EndScene() {
		PP_PROFILE_FUNCTION();

	}

	/// Draw Flat ///
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		PP_PROFILE_FUNCTION();

		m_Data->m_TextureShader->Bind();
		m_Data->m_TextureShader->SetVec4("Color", color);
		m_Data->m_WhiteTexture->Bind(0);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		m_Data->m_TextureShader->SetMat4("Model", model);
		RenderCommand::DrawIndexed(m_Data->m_VertexArray);
	}

	/// Draw Texture ///
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float filingFactor, const glm::vec4& tintColor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, filingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float filingFactor, const glm::vec4& tintColor) {
		PP_PROFILE_FUNCTION();

		m_Data->m_TextureShader->Bind();
		m_Data->m_TextureShader->SetVec4("Color", tintColor);
		m_Data->m_TextureShader->SetFloat("FilingFactor", filingFactor);
		texture->Bind(0);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		m_Data->m_TextureShader->SetMat4("Model", model);
		RenderCommand::DrawIndexed(m_Data->m_VertexArray);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
		PP_PROFILE_FUNCTION();

		m_Data->m_TextureShader->Bind();
		m_Data->m_TextureShader->SetVec4("Color", color);
		m_Data->m_WhiteTexture->Bind(0);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		m_Data->m_TextureShader->SetMat4("Model", model);
		RenderCommand::DrawIndexed(m_Data->m_VertexArray);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float filingFactor /*= 1*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/) {
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, texture, filingFactor, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float filingFactor /*= 1*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/) {
		PP_PROFILE_FUNCTION();

		m_Data->m_TextureShader->Bind();
		m_Data->m_TextureShader->SetVec4("Color", tintColor);
		m_Data->m_TextureShader->SetFloat("FilingFactor", filingFactor);
		texture->Bind(0);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		m_Data->m_TextureShader->SetMat4("Model", model);
		RenderCommand::DrawIndexed(m_Data->m_VertexArray);
	}

}