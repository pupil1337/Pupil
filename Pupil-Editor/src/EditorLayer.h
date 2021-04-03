#pragma once
#include "pupil/Core/Core.h"
#include "Pupil.h"

#include "ParticleSystem.h"

namespace Pupil {

	class PP_API EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

	private:
		Ref<Shader> m_Shader;
		Ref<Framebuffer> m_Framebuffer;
		Ref<VertexArray>  m_VertexArray;
		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		Ref<Scene> m_Scene;
		Entity m_SquareEntity;
		Entity m_Camera, m_ClipCamera;
		bool m_PrimaryCamera = true;

		OrthographicCameraController m_OrthoCameraController;

		glm::vec4 m_Color;
		Ref<Texture2D> m_Texture1;
		Ref<Texture2D> m_Texture2;
		Ref<Texture2D> m_Texture3;
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_SoilTexture, m_WaterTexture, m_TreeTexture;
		std::unordered_map<char, Ref<SubTexture2D>> m_SpriteMap;

		TimeStep m_TimeStep;
		float m_FrameTimeGraph[100] = { 0.0f };
		uint32_t values_offset = 0;

		// Particle
		ParticleProps m_Particle;
		ParticleSystem m_ParticleSystem;
	};
}