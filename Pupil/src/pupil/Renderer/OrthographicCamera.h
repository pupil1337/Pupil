#pragma once
#include "pupil/Core/Core.h"

#include <glm/glm.hpp>

namespace Pupil {

	class PP_API OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetPorjection(float left, float right, float bottom, float top);

		// position
		inline void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateProjectionViewModel(); }
		inline const glm::vec3& GetPosition() const { return m_Position; }

		// rotate
		inline void SetRotation(float rotation) { m_Rotation = rotation; RecalculateProjectionViewModel(); }
		inline float GetRotation() const { return m_Rotation; }
		
		// ---matrix---
		inline const glm::mat4& GetView() const { return m_View; }
		inline const glm::mat4& GetProjection() const { return m_Projection; }
		inline const glm::mat4& GetProjectionView() const { return m_ProjectionView; }

	private:
		void RecalculateProjectionViewModel();
	private:
		glm::mat4 m_View;
		glm::mat4 m_Projection;
		glm::mat4 m_ProjectionView;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f};
		float m_Rotation = 0.0f;
	};
}