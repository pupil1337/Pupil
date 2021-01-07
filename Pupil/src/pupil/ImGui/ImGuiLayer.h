#pragma once

#include "pupil/Layer.h"
#include "pupil/Events/MouseEvent.h"
#include "pupil/Events/KeyEvent.h"
#include "pupil/Events/ApplicationEvent.h"

namespace Pupil {

	class PP_API ImGuiLayer : public Layer{

	public:
		ImGuiLayer();
		~ImGuiLayer();

<<<<<<< HEAD
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnImGuiRender() override;

		void Begin();
		void End();
=======
		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizedEvent(WindowResizedEvent& e);
>>>>>>> fbb57b5050c9e8a49872d13e72b5d6ea531c356a

	private:
		float m_Time = 0.0f;

	};
}