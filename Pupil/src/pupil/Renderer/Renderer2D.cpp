#include "pppch.h"
#include "Renderer2D.h"

#include "platform/OpenGL/OpenGLShader.h"
#include "RenderCommand.h"

namespace Pupil {

	Scope<Renderer2D::Renderer2DStorage> Renderer2D::m_Data = std::make_unique<Renderer2D::Renderer2DStorage>();
	
	void Renderer2D::Init() {

		float vertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f
		};
		m_Data->m_VertexArray = Pupil::VertexArray::Create();

		Pupil::Ref<Pupil::VertexBuffer> vertexBuffer = Pupil::VertexBuffer::Create(vertices, sizeof(vertices));
		Pupil::BufferLayout layout = {
			{ Pupil::ShaderDataType::Float3, "aPos"}
		};
		vertexBuffer->SetLayout(layout);
		m_Data->m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 1, 3, 2 };
		Pupil::Ref<Pupil::IndexBuffer> indexBuffer = Pupil::Ref<Pupil::IndexBuffer>(Pupil::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_Data->m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Data->m_Shader = Shader::Create("assets/shaders/simple");
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		m_Data->m_Shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_Data->m_Shader)->SetMat4("ProjectionView", camera.GetProjectionView());
	}

	void Renderer2D::EndScene() {
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad(glm::vec3(position, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		m_Data->m_Shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_Data->m_Shader)->SetVec4("Color", color);
		std::dynamic_pointer_cast<OpenGLShader>(m_Data->m_Shader)->SetMat4("Model", glm::mat4(1.0f));
		RenderCommand::DrawIndexed(m_Data->m_VertexArray);
	}

}