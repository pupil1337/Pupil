#include "pppch.h"
#include "OrthographicCameraController.h"

#include "pupil/Core/Input.h"
#include "pupil/Core/KeyCodes.h"

namespace Pupil {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation /*= false*/)
		: m_AspectRatio(aspectRatio), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), m_Rotation(rotation) {

	}

	void OrthographicCameraController::OnUpdate(TimeStep ts) {
		PP_PROFILE_FUNCTION();

		if (Pupil::Input::IsKeyPressed(PP_KEY_A)) {
			m_CameraPosition.x -= cos(glm::radians(-m_CameraRotation)) * m_ZoomLevel * ts;
			m_CameraPosition.y -= sin(glm::radians(-m_CameraRotation)) * m_ZoomLevel * ts;
		}
		if (Pupil::Input::IsKeyPressed(PP_KEY_D)) {
			m_CameraPosition.x += cos(glm::radians(-m_CameraRotation)) * m_ZoomLevel * ts;
			m_CameraPosition.y += sin(glm::radians(-m_CameraRotation)) * m_ZoomLevel * ts;
		}
		if (Pupil::Input::IsKeyPressed(PP_KEY_S)) {
			m_CameraPosition.x += sin(glm::radians(-m_CameraRotation)) * m_ZoomLevel * ts;
			m_CameraPosition.y -= cos(glm::radians(-m_CameraRotation)) * m_ZoomLevel * ts;
		}
		if (Pupil::Input::IsKeyPressed(PP_KEY_W)) {
			m_CameraPosition.x -= sin(glm::radians(-m_CameraRotation)) * m_ZoomLevel * ts;
			m_CameraPosition.y += cos(glm::radians(-m_CameraRotation)) * m_ZoomLevel * ts;
		}
		m_Camera.SetPosition(m_CameraPosition);
		//PP_TRACE("{0}, {1}", m_CameraPosition.x, m_CameraPosition.y);

		if (m_Rotation) {
			if (Pupil::Input::IsKeyPressed(PP_KEY_Q)) m_CameraRotation -= 90.0f * ts;
			if (Pupil::Input::IsKeyPressed(PP_KEY_E)) m_CameraRotation += 90.0f * ts;
			m_Camera.SetRotation(m_CameraRotation);
		}
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		PP_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(PP_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizedEvent>(PP_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		PP_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.125f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.125f);
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
		return true;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& e) {
		PP_PROFILE_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
		return true;
	}

}