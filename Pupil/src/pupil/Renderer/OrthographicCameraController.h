#pragma once
#include "pupil/Core/Core.h"

#include "pupil/Renderer/OrthographicCamera.h"
#include "pupil/Core/TimeStep.h"
#include "pupil/Events/Event.h"
#include "pupil/Events/ApplicationEvent.h"
#include "pupil/Events/MouseEvent.h"

namespace Pupil {

	struct OrthographicCameraBounds {
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class PP_API OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		inline const OrthographicCamera& GetCamera() { return m_Camera; }

		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		bool m_Rotation;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
	};
}