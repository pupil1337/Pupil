#pragma once
#include "pupil/Core/Core.h"
#include "Pupil.h"

#include "pupil/Core/Layer.h"

#include "ParticleSystem.h"

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
		Pupil::Ref<Pupil::Framebuffer> m_Framebuffer;
		Pupil::Ref<Pupil::VertexArray>  m_VertexArray;

		Pupil::OrthographicCameraController m_OrthoCameraController;

		glm::vec4 m_Color;
		Pupil::Ref<Texture2D> m_Texture1;
		Pupil::Ref<Texture2D> m_Texture2;
		Pupil::Ref<Texture2D> m_Texture3;
		Pupil::Ref<Texture2D> m_SpriteSheet;
		Pupil::Ref<SubTexture2D> m_SoilTexture, m_WaterTexture, m_TreeTexture;
		std::unordered_map<char, Pupil::Ref<SubTexture2D>> m_SpriteMap;

		Pupil::TimeStep m_TimeStep;
		float m_FrameTimeGraph[100] = { 0.0f };
		uint32_t values_offset = 0;

		// Particle
		ParticleProps m_Particle;
		ParticleSystem m_ParticleSystem;
	};
}