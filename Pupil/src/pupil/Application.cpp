#include "pppch.h"
#include "Application.h"

#include "glad/glad.h"

#include "pupil/Input.h"

#include "pupil/Renderer/Renderer.h"


namespace Pupil {

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		
		PP_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(PP_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		// -------------------------------------

		float vertices[4 * 7] = {
			-0.5f, -0.5f, 0.0f,  0.8f,  0.2f, 0.8f, 1.0f,
			-0.5f,  0.5f, 0.0f,  0.8f,  0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f,  0.8f,  0.2f, 0.8f, 1.0f,
			 0.5f,  0.5f, 0.0f,  0.8f,  0.2f, 0.8f, 1.0f
		};
		m_VertexArray.reset(VertexArray::Create());

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "aPos"},
			{ ShaderDataType::Float4, "aColor"}
		};
		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 1, 3, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		// --shader-----------------------------------------
		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 aPos;
			layout(location = 1) in vec4 aColor;

			out vec3 FragPos;			
			out vec4 Color;
			
			void main() {
				FragPos = aPos;
				Color = aColor;
				gl_Position = vec4(aPos, 1.0f);
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


		m_Shader = std::unique_ptr<Shader>(new Pupil::Shader(vertexSrc, fragmentSrc));
	}
	
	Application::~Application() {
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlayer(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(PP_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
			(*it)->OnEvent(e);
			if (e.m_Handled) break;
			
		}

	}

	void Application::Run() {

		m_Shader->Bind();
		while (m_Running) {
			RenderCommand::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
			RenderCommand::Clear();

			Renderer::BeginScene();

			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowClosedEvent& e) {
		m_Running = false;
		return true;
	}
}