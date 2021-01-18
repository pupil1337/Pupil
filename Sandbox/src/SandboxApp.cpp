#include "pppch.h"
#include "Pupil.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer :public Pupil::Layer {
public:
	ExampleLayer()
		:Layer("Example"), m_OrthoCamera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {

		float vertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
		};
		m_VertexArray.reset(Pupil::VertexArray::Create());

		m_VertexBuffer.reset(Pupil::VertexBuffer::Create(vertices, sizeof(vertices)));
		Pupil::BufferLayout layout = {
			{ Pupil::ShaderDataType::Float3, "aPos"}
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 1, 3, 2 };
		m_IndexBuffer.reset(Pupil::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// --shader-----------------------------------------
		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 aPos;
			
			uniform mat4 ProjectonView;
			uniform mat4 Model;
			
			void main() {
				gl_Position = ProjectonView * Model * vec4(aPos, 1.0f);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			out vec4 FragColor;
			
			uniform vec3 Color;
			
			void main() {
				FragColor = vec4(Color, 1.0f);
			}
		)";


		m_Shader = std::unique_ptr<Pupil::Shader>(Pupil::Shader::Create(vertexSrc, fragmentSrc));
	}

	virtual void OnUpdate(Pupil::TimeStep ts) override {

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

		//Pupil::Material gold = Pupil::GetMaterial(Pupil::Material_BPhong_Type::gold);
		m_Shader->Bind();
		std::dynamic_pointer_cast<Pupil::OpenGLShader>(m_Shader)->SetVec3("Color", m_Color);
		m_Shader->UnBind();
		for (int i = 0; i != 8; ++i) {
			for (int j = 0; j != 8; ++j) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i*0.205f, j*0.205f, 0.0f));
				model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
				model = glm::scale(model, glm::vec3(0.2f));
				Pupil::Renderer::Submit(m_Shader, m_VertexArray, model);
			}
		}
		Pupil::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Color", glm::value_ptr(m_Color));
		ImGui::End();
	}

	virtual void OnEvent(Pupil::Event& event) override {

	}

private:
	unsigned int m_VertexArrays;
	Pupil::Ref<Pupil::Shader> m_Shader;
	Pupil::Ref<Pupil::VertexArray>  m_VertexArray;
	Pupil::Ref<Pupil::VertexBuffer>  m_VertexBuffer;
	Pupil::Ref<Pupil::IndexBuffer>  m_IndexBuffer;

	glm::vec3 m_Color = glm::vec3(0.8f, 0.2f, 0.8f);

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