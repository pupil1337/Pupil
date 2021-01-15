#include "pppch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Pupil {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:m_Projection(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_View(1.0f) {
		
		m_ProjectionView = m_Projection * m_View;
	}

	void OrthographicCamera::RecalculateProjectionViewModel() {
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, -1.0f));

		m_View = glm::inverse(trans);
		m_ProjectionView = m_Projection * m_View;
	}

}