#include "pppch.h"
#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "pupil/Core/Input.h"

#include "pupil/Renderer/Renderer.h"

#include "pupil/Renderer/Shader.h"
#include "pupil/Renderer/Buffer.h"
#include "pupil/Renderer/VertexArray.h"

#include "pupil/Renderer/OrthographicCamera.h"


namespace Pupil {

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		
		PP_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		
		m_Window = Scope<Window>(Window::Create());
		m_Window->SetEventCallback(PP_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

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
		dispatcher.Dispatch<WindowResizedEvent>(PP_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
			(*it)->OnEvent(e);
			if (e.m_Handled) break;
			
		}

	}

	void Application::Run() {

		while (m_Running) {
			float time = (float)glfwGetTime();
			TimeStep timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_MiniSize) {
				for (Layer* layer : m_LayerStack) {
					layer->OnUpdate(timeStep);
				}
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

	bool Application::OnWindowResize(WindowResizedEvent& e) {
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_MiniSize = true;
		}
		else {
			RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
			m_MiniSize = false;
		}
		return false;
	}
}