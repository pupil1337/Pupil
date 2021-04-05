#include "pppch.h"
#include "SceneCamera.h"

namespace Pupil {
	
	SceneCamera::SceneCamera() {
		CalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
		m_AspectRatio = (float)width / (float)height;
		CalculateProjection();
	}

	void SceneCamera::CalculateProjection() {
		if (m_ProjectionType == ProjectionType::Perspective) {
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspcetiveFar);
		}
		else {
			float left = -m_OrthographicSize * m_AspectRatio * 0.5;
			float right = m_OrthographicSize * m_AspectRatio * 0.5;
			float bottom = -m_OrthographicSize * 0.5;
			float top = m_OrthographicSize * 0.5;
			m_Projection = glm::ortho(left, right, bottom, top);
		}
	}

	void SceneCamera::SetPersepctive(float Fov, float Near, float Far) {
		m_PerspectiveFOV = Fov;
		m_PerspectiveNear = Near;
		m_PerspcetiveFar = Far;
		m_ProjectionType = ProjectionType::Perspective;
		CalculateProjection();
	}

	void SceneCamera::SetOrthographic(uint32_t size, float clipNear, float clipFar) {
		m_OrthographicSize = size;
		m_OrthographicNear = clipNear;
		m_OrthographicFar = clipFar;
		m_ProjectionType = ProjectionType::Orthographic;
		CalculateProjection();
	}

}