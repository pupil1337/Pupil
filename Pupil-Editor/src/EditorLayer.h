#pragma once
#include "pupil/Core/Core.h"
#include "Pupil.h"

#include "ParticleSystem.h"
#include "panel/ScenePanel.h"
#include "pupil/Scene/SceneSerializer.h"
#include "pupil/Renderer/EditorCamera.h"

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
		bool OnKeyPressed(KeyPressedEvent& e);
		//bool OnMousePressed(MouseButtonPressedEvent& event);
		void ViewportEvent();
		void NewScene();
		void OpenScene();
		void SaveScene();

	private:
		Ref<Shader> m_Shader;
		Ref<Framebuffer> m_Framebuffer;
		
		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportPos;

		Ref<Scene> m_Scene;
		ScenePanel m_ScenePanel;
		EditorCamera m_EditorCamera;
		int m_GizmoType = -1;
		
		OrthographicCameraController m_OrthoCameraController;

		// Performance
		TimeStep m_TimeStep;
		float m_FrameTimeGraph[100] = { 0.0f };
		uint32_t values_offset = 0;

		// Particle
		ParticleProps m_Particle;
		ParticleSystem m_ParticleSystem;
		
		// ToDo 过时的东西，其中有些将要加到"component"里面 
		// Entity m_RedEntity, m_GreenEntity;
		// Entity m_Camera, m_ClipCamera;
		// bool m_PrimaryCamera = true;
		// 
		// Ref<VertexArray>  m_VertexArray;
		// 
		// glm::vec4 m_Color;
		// Ref<Texture2D> m_Texture1;
		// Ref<Texture2D> m_Texture2;
		// Ref<Texture2D> m_Texture3;
		// Ref<Texture2D> m_SpriteSheet;
		// Ref<SubTexture2D> m_SoilTexture, m_WaterTexture, m_TreeTexture;
		// std::unordered_map<char, Ref<SubTexture2D>> m_SpriteMap;
	};
}