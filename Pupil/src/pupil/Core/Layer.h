#pragma once

#include "pupil/Core/Core.h"
#include "pupil/Events/Event.h"

#include "pupil/Core/TimeStep.h"

namespace Pupil {
	
	class PP_API Layer {
	
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() { }
		virtual void OnDetach() { }
		virtual void OnUpdate(Pupil::TimeStep ts) { }
		virtual void OnImGuiRender() { }
		virtual void OnEvent(Event& event) { }
		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;

	};
}