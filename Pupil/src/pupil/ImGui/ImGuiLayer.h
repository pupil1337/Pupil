#pragma once

#include "pupil/Core/Layer.h"
#include "pupil/Events/MouseEvent.h"
#include "pupil/Events/KeyEvent.h"
#include "pupil/Events/ApplicationEvent.h"

namespace Pupil {

	class PP_API ImGuiLayer : public Layer{

	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		// on update
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void BlockEvent(bool block) { m_BlockEvent = block; }

		void SetDarkThemeColor();
	private:
		float m_Time = 0.0f;
		bool m_BlockEvent = false;
	};
}