#pragma once
#include "pupil/Core/Core.h"
#include "Pupil.h"

#include "pupil/Core/Layer.h"

namespace Pupil {

	class PP_API Sandbox2D : public Pupil::Layer {
	public:
		Sandbox2D();
		virtual ~Sandbox2D() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Pupil::TimeStep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Pupil::Event& event) override;

	private:
		Pupil::Ref<Pupil::Shader> m_Shader;
		Pupil::Ref<Pupil::VertexArray>  m_VertexArray;

		Pupil::OrthographicCameraController m_OrthoCameraController;

		glm::vec4 m_Color;

		Pupil::TimeStep m_TimeStep;
		float m_FrameTimeGraph[100] = { 0.0f };
		uint32_t values_offset = 0;
	};
}