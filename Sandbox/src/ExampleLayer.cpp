#include "pppch.h"
#include "ExampleLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Pupil {
	ExampleLayer::ExampleLayer()
		:Layer("Example"), m_OrthoCameraController(1280.0f / 720.0f, true) {
	}

	void ExampleLayer::OnAttach() {
		m_ShaderLibrary.Load("TextureShader", "assets/shaders/Texture");

		float vertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f
		};
		glm::mat4* modelMatrixs = new glm::mat4[m_row * m_col];
		for (int i = 0; i != m_row; ++i) {
			for (int j = 0; j != m_col; ++j) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i * 0.205f, j * 0.205f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
				model = glm::scale(model, glm::vec3(0.2f));
				modelMatrixs[i * j] = model;
			}
		}
		m_VertexArray = Pupil::VertexArray::Create();

		Pupil::Ref<Pupil::VertexBuffer> vertexBuffer = Pupil::VertexBuffer::Create(vertices, sizeof(vertices));
		Pupil::BufferLayout layout = {
			{ Pupil::ShaderDataType::Float3, "aPos"},
			{ Pupil::ShaderDataType::Float2, "aTexCoord"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 1, 3, 2 };
		Pupil::Ref<Pupil::IndexBuffer> indexBuffer = Pupil::Ref<Pupil::IndexBuffer>(Pupil::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Shader = m_ShaderLibrary.GetShader("TextureShader");

		m_Texture2D1 = Pupil::Texture2D::Create("assets/textures/container2.png");
		m_Texture2D2 = Pupil::Texture2D::Create("assets/textures/awesomeface.png");

		m_Shader->Bind();
		m_Shader->SetInt("Texture0", 0);
		m_Shader->SetVec4("Color", glm::vec4(1.0f));
		m_Shader->SetFloat("FilingFactor", 1.0f);

		// white texture
		uint32_t WhiteData = 0xffffffff;
		m_WhiteTexture = Texture2D::Create(1, 1);
		m_WhiteTexture->SetData(&WhiteData, sizeof(uint32_t));

	}

	void ExampleLayer::OnDetach() {

	}

	void ExampleLayer::OnUpdate(Pupil::TimeStep ts) {
		m_TimeStep = ts;

		m_OrthoCameraController.OnUpdate(ts);

		Pupil::RenderCommand::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
		Pupil::RenderCommand::Clear();

		Pupil::Renderer::BeginScene(m_OrthoCameraController.GetCamera());

		m_Shader->Bind();
		m_Shader->SetVec4("Color", glm::vec4(1.0f));
		// -------1
		m_Texture2D2->Bind();
		for (int i = 0; i != m_row; ++i) {
			for (int j = 0; j != m_col; ++j) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i * 0.205f, j * 0.205f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
				model = glm::scale(model, glm::vec3(0.2f));
				Pupil::Renderer::Submit(m_Shader, m_VertexArray, model);
			}
		}
		// -------2
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.5f));

		m_Texture2D1->Bind();
		Pupil::Renderer::Submit(m_Shader, m_VertexArray, model);
		// -------3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 0.0f));
		m_WhiteTexture->Bind();
		m_Shader->SetVec4("Color", color);
		Pupil::Renderer::Submit(m_Shader, m_VertexArray, model);

		Pupil::Renderer::EndScene();
	}

	void ExampleLayer::OnImGuiRender() {
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Color", glm::value_ptr(color));
		ImGui::End();

		// DragInt
		ImGui::Begin("row & col");
		ImGui::DragInt("Row: ", &m_row, 1, 1, 100);
		ImGui::DragInt("Row: ", &m_col, 1, 1, 100);
		ImGui::End();

		// performance PlotLines
		ImGui::Begin("Renderer Performance");
		m_FrameTimeGraph[values_offset] = m_TimeStep.GetMilliSecond();
		values_offset = (values_offset + 1) % 100;
		ImGui::PlotLines("#FrameTime", m_FrameTimeGraph, 100, values_offset, "FrameTime (ms)", 0.0f, 200.0f, ImVec2(0, 100), 100);
		ImGui::Text("FrameTime: %.2f (Fps: %d)", m_TimeStep.GetMilliSecond(), (int)(1.0f / m_TimeStep.GetSecond()));
		ImGui::End();
	}

	void ExampleLayer::OnEvent(Pupil::Event& event) {
		m_OrthoCameraController.OnEvent(event);
	}
}