#pragma once

#include "Core.h"

#include "Window.h"

#include "pupil/Events/Event.h"
#include "pupil/Events/ApplicationEvent.h"

#include "pupil/Core/LayerStack.h"

#include "pupil/ImGui/ImGuiLayer.h"

namespace Pupil {

	class PP_API Application {
	public:
		Application(const std::string& name);
		virtual ~Application();

		void Run();
		void Close() { m_Running = false; }

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowClosedEvent& e);
		bool OnWindowResize(WindowResizedEvent& e);

		Scope<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_MiniSize = false;

		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();

}