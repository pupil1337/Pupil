#pragma once
#include "pupil/Core/Core.h"

#include "pupil/Renderer/OrthographicCamera.h"
#include "pupil/Core/TimeStep.h"
#include "pupil/Events/Event.h"
#include "pupil/Events/ApplicationEvent.h"
#include "pupil/Events/MouseEvent.h"

namespace Pupil {

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		inline const OrthographicCamera& GetCamera() { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		bool m_Rotation;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
	};
}