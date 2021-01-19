#include "pppch.h"
#include "Pupil.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer :public Pupil::Layer {
public:
	ExampleLayer()
		:Layer("Example"), m_OrthoCamera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {

		Pupil::Renderer::Init();
		m_ShaderLibrary.Load("TextureShader", "assets/shaders/Texture");

		float vertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f
		};
		m_VertexArray = Pupil::VertexArray::Create();

		m_VertexBuffer = Pupil::VertexBuffer::Create(vertices, sizeof(vertices));
		Pupil::BufferLayout layout = {
			{ Pupil::ShaderDataType::Float3, "aPos"},
			{ Pupil::ShaderDataType::Float2, "aTexCoord"}
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 1, 3, 2 };
		m_IndexBuffer = Pupil::Ref<Pupil::IndexBuffer>(Pupil::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_Shader = m_ShaderLibrary.GetShader("TextureShader");
		
		m_Texture2D1 = Pupil::Texture2D::Create("assets/textures/container.jpg");
		m_Texture2D2 = Pupil::Texture2D::Create("assets/textures/awesomeface.png");

		m_Shader->Bind();
		std::dynamic_pointer_cast<Pupil::OpenGLShader>(m_Shader)->SetInt("Texture1", 0);
		m_Shader->UnBind();

	}

	virtual void OnUpdate(Pupil::TimeStep ts) override {
		//PP_TRACE("Fps:{0}", 1.0f / ts);
		if (Pupil::Input::IsKeyPressed(PP_KEY_A)) m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		if (Pupil::Input::IsKeyPressed(PP_KEY_D)) m_CameraPosition.x += m_CameraMoveSpeed * ts;
		if (Pupil::Input::IsKeyPressed(PP_KEY_S)) m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		if (Pupil::Input::IsKeyPressed(PP_KEY_W)) m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Pupil::Input::IsKeyPressed(PP_KEY_Q)) m_CameraRotation -= m_CameraRotateSpeed * ts;
		if (Pupil::Input::IsKeyPressed(PP_KEY_E)) m_CameraRotation += m_CameraRotateSpeed * ts;


		Pupil::RenderCommand::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
		Pupil::RenderCommand::Clear();

		m_OrthoCamera.SetPosition(m_CameraPosition);
		m_OrthoCamera.SetRotation(m_CameraRotation);

		Pupil::Renderer::BeginScene(m_OrthoCamera);

		for (int i = 0; i != 8; ++i) {
			for (int j = 0; j != 8; ++j) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i*0.205f, j*0.205f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
				model = glm::scale(model, glm::vec3(0.2f));
				m_Texture2D2->Bind();
				Pupil::Renderer::Submit(m_Shader, m_VertexArray, model);
			}
		}
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f));

		m_Texture2D1->Bind();
		Pupil::Renderer::Submit(m_Shader, m_VertexArray, model);
		m_Texture2D2->Bind();
		Pupil::Renderer::Submit(m_Shader, m_VertexArray, model);
		Pupil::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		//ImGui::ColorEdit3("Color", glm::value_ptr(m_Color));
		ImGui::End();
	}

	virtual void OnEvent(Pupil::Event& event) override {

	}

private:
	Pupil::ShaderLibrary m_ShaderLibrary;
	Pupil::Ref<Pupil::Shader> m_Shader;
	Pupil::Ref<Pupil::VertexArray>  m_VertexArray;
	Pupil::Ref<Pupil::VertexBuffer>  m_VertexBuffer;
	Pupil::Ref<Pupil::IndexBuffer>  m_IndexBuffer;

	Pupil::Ref<Pupil::Texture2D> m_Texture2D1;
	Pupil::Ref<Pupil::Texture2D> m_Texture2D2;

	Pupil::OrthographicCamera m_OrthoCamera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraMoveSpeed = 1.0f;
	float m_CameraRotateSpeed = 90.0f;
};

class Sandbox : public Pupil::Application {

public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}

};

Pupil::Application* Pupil::CreateApplication() {
	return new Sandbox();
}