#include "pppch.h"
#include "Renderer2D.h"

#include "platform/OpenGL/OpenGLShader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Pupil {

	Scope<Renderer2D::Renderer2DStorage> Renderer2D::m_Data = std::make_unique<Renderer2D::Renderer2DStorage>();
	
	void Renderer2D::Init() {

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

		m_Data->m_SimpleShader = Shader::Create("assets/shaders/Simple");
		m_Data->m_TextureShader = Shader::Create("assets/shaders/Texture");
		m_Data->m_TextureShader->Bind();
		m_Data->m_TextureShader->SetInt("Texture0", 0);
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		m_Data->m_SimpleShader->Bind();
		m_Data->m_SimpleShader->SetMat4("ProjectionView", camera.GetProjectionView());
		
		m_Data->m_TextureShader->Bind();
		m_Data->m_TextureShader->SetMat4("ProjectionView", camera.GetProjectionView());
	}

	void Renderer2D::EndScene() {
	}

	/// Draw Flat ///
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		m_Data->m_SimpleShader->Bind();
		m_Data->m_SimpleShader->SetVec4("Color", color);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		m_Data->m_SimpleShader->SetMat4("Model", model);
		RenderCommand::DrawIndexed(m_Data->m_VertexArray);
	}

	/// Draw Texture ///
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
		m_Data->m_TextureShader->Bind();
		texture->Bind(0);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		m_Data->m_TextureShader->SetMat4("Model", model);
		RenderCommand::DrawIndexed(m_Data->m_VertexArray);
	}

}