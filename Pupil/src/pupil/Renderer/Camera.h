#pragma once
#include "pupil/Core/Core.h"

#include <glm/glm.hpp>

namespace Pupil {

	class Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4& projection): Projection(projection) {  }

		virtual ~Camera() = default;

		virtual const glm::mat4& GetProjection() const { return Projection; }

	protected:
		glm::mat4 Projection;

	};

}