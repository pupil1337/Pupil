#include "pppch.h"
#include "OrthographicCameraController.h"

#include "pupil/Input.h"
#include "pupil/KeyCodes.h"

namespace Pupil {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation /*= false*/)
	: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation) {
		
	}

	void OrthographicCameraController::OnUpdate(TimeStep ts) {
		if (Pupil::Input::IsKeyPressed(PP_KEY_A)) m_CameraPosition.x -= m_ZoomLevel * ts;
		if (Pupil::Input::IsKeyPressed(PP_KEY_D)) m_CameraPosition.x += m_ZoomLevel * ts;
		if (Pupil::Input::IsKeyPressed(PP_KEY_S)) m_CameraPosition.y -= m_ZoomLevel * ts;
		if (Pupil::Input::IsKeyPressed(PP_KEY_W)) m_CameraPosition.y += m_ZoomLevel * ts;
		m_Camera.SetPosition(m_CameraPosition);
		//PP_TRACE("{0}, {1}", m_CameraPosition.x, m_CameraPosition.y);

		if (m_Rotation) {
			if (Pupil::Input::IsKeyPressed(PP_KEY_Q)) m_CameraRotation -= 90.0f * ts;
			if (Pupil::Input::IsKeyPressed(PP_KEY_E)) m_CameraRotation += 90.0f * ts;
			m_Camera.SetRotation(m_CameraRotation);
		}
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(PP_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizedEvent>(PP_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		m_ZoomLevel -= e.GetYOffset() * 0.125f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.125f);
		m_Camera.SetPorjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return true;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& e) {
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetPorjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return true;
	}

}