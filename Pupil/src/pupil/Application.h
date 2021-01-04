#pragma once

#include"Core.h"

#include"Window.h"
#include"pupil/Events/Event.h"
#include"pupil/Events/ApplicationEvent.h"

#include"pupil/LayerStack.h"

namespace Pupil {

	class PP_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();

}