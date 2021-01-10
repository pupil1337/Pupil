#pragma once

#include "Core.h"

#include "Window.h"

#include "pupil/Events/Event.h"
#include "pupil/Events/ApplicationEvent.h"

#include "pupil/LayerStack.h"

#include "pupil/ImGui/ImGuiLayer.h"

#include "pupil/Renderer/Shader.h"

namespace Pupil {

	class PP_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowClosedEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArrays, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();

}