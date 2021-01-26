#pragma once
#include "Pupil.h"

namespace Pupil {
	class ExampleLayer :public Pupil::Layer {
	public:
		ExampleLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Pupil::TimeStep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Pupil::Event& event) override;

	private:
		Pupil::ShaderLibrary m_ShaderLibrary;
		Pupil::Ref<Pupil::Shader> m_Shader;
		Pupil::Ref<Pupil::VertexArray>  m_VertexArray;
		Pupil::Ref<Pupil::VertexBuffer>  m_VertexBuffer;
		Pupil::Ref<Pupil::IndexBuffer>  m_IndexBuffer;

		Pupil::Ref<Pupil::Texture2D> m_Texture2D1;
		Pupil::Ref<Pupil::Texture2D> m_Texture2D2;
		Pupil::Ref<Texture2D> m_WhiteTexture;

		Pupil::OrthographicCameraController m_OrthoCameraController;

		int m_row = 1, m_col = 1;
		glm::vec4 color = { 0.8f, 0.8f, 0.2f, 1.0f };

		Pupil::TimeStep m_TimeStep;
		float m_FrameTimeGraph[100];
		uint32_t values_offset = 0;
	};
}