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

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;

	};
}