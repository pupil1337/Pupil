#include "pppch.h"
#include "Pupil.h"

#include "imgui/imgui.h"

class ExampleLayer :public Pupil::Layer {
public:
	ExampleLayer()
		:Layer("Example"), m_OrthoCamera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f) {

		float vertices[4 * 7] = {
			-0.5f, -0.5f, 0.0f,  0.8f,  0.2f, 0.8f, 1.0f,
			-0.5f,  0.5f, 0.0f,  0.8f,  0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f,  0.8f,  0.2f, 0.8f, 1.0f,
			 0.5f,  0.5f, 0.0f,  0.8f,  0.2f, 0.8f, 1.0f
		};
		m_VertexArray.reset(Pupil::VertexArray::Create());

		m_VertexBuffer.reset(Pupil::VertexBuffer::Create(vertices, sizeof(vertices)));
		Pupil::BufferLayout layout = {
			{ Pupil::ShaderDataType::Float3, "aPos"},
			{ Pupil::ShaderDataType::Float4, "aColor"}
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
			layout(location = 1) in vec4 aColor;
			
			uniform mat4 view;
			
			out vec3 FragPos;			
			out vec4 Color;
			
			void main() {
				FragPos = aPos;
				Color = aColor;
				gl_Position = view * vec4(aPos, 1.0f);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			out vec4 FragColor;

			in vec3 FragPos;
			in vec4 Color;
			
			void main() {
				FragColor = vec4(Color);
			}
		)";


		m_Shader = std::unique_ptr<Pupil::Shader>(new Pupil::Shader(vertexSrc, fragmentSrc));
	}

	virtual void OnUpdate() override {
		if (Pupil::Input::IsKeyPressed(PP_KEY_A)) m_CameraPosition.x -= m_CameraMoveSpeed;
		if (Pupil::Input::IsKeyPressed(PP_KEY_D)) m_CameraPosition.x += m_CameraMoveSpeed;
		if (Pupil::Input::IsKeyPressed(PP_KEY_S)) m_CameraPosition.y -= m_CameraMoveSpeed;
		if (Pupil::Input::IsKeyPressed(PP_KEY_W)) m_CameraPosition.y += m_CameraMoveSpeed;

		if (Pupil::Input::IsKeyPressed(PP_KEY_Q)) m_CameraRotation -= m_CameraRotateSpeed;
		if (Pupil::Input::IsKeyPressed(PP_KEY_E)) m_CameraRotation += m_CameraRotateSpeed;



		Pupil::RenderCommand::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
		Pupil::RenderCommand::Clear();

		m_OrthoCamera.SetPosition(m_CameraPosition);
		m_OrthoCamera.SetRotation(m_CameraRotation);

		Pupil::Renderer::BeginScene(m_OrthoCamera);

		Pupil::Renderer::Submit(m_Shader, m_VertexArray);

		Pupil::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	virtual void OnEvent(Pupil::Event& event) override {

	}

private:
	unsigned int m_VertexArrays;
	std::shared_ptr<Pupil::Shader> m_Shader;
	std::shared_ptr<Pupil::VertexArray>  m_VertexArray;
	std::shared_ptr<Pupil::VertexBuffer>  m_VertexBuffer;
	std::shared_ptr<Pupil::IndexBuffer>  m_IndexBuffer;

	Pupil::OrthographicCamera m_OrthoCamera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.01;

	float m_CameraRotation = 0.01;
	float m_CameraRotateSpeed = 1.0f;
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