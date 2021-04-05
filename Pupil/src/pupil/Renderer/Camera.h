#pragma once
#include "pupil/Core/Core.h"

#include <glm/glm.hpp>

namespace Pupil {

	class Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4& projection): m_Projection(projection) {  }

		virtual ~Camera() = default;

		virtual const glm::mat4& GetProjection() const { return m_Projection; }

	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);

	};

}