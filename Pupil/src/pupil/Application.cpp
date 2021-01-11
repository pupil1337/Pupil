#include "pppch.h"
#include "Application.h"

#include "glad/glad.h"

#include "pupil/Input.h"

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

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f
		};
		uint32_t indices[3] = { 0, 1, 2 };

		glGenVertexArrays(1, &m_VertexArrays);
		glBindVertexArray(m_VertexArrays);

		m_VertexBuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
		m_VertexBuffer->Bind();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		
		m_IndexBuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_IndexBuffer->Bind();

		// --shader-----------------------------------------
		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 aPos;

			out vec3 FragPos;			

			void main() {
				FragPos = aPos;
				gl_Position = vec4(aPos, 1.0f);
			}
		)";
		
		std::string fragmentSrc = R"(
			#version 330 core
			out vec4 FragColor;

			in vec3 FragPos;
			
			void main() {
				FragColor = vec4(FragPos + 0.5f, 1.0f);
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
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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