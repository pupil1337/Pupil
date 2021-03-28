#pragma once
#include "pupil/Core/Core.h"

#include <glm/glm.hpp>

namespace Pupil {

	struct TransformComponent {
		TransformComponent() = default;
		TransformComponent(const glm::mat4& transform) : Transform(transform) { }

		TransformComponent(const TransformComponent& transform) = default;

		TransformComponent& operator=(TransformComponent & rhs) {
			Transform = rhs.Transform;
			return *this;
		}

		operator glm::mat4() { return Transform; }
		operator const glm::mat4() const { return Transform; }

		/// data ///
		glm::mat4 Transform{1.0f};
	};


	struct ColorComponent {
		ColorComponent() = default;
		ColorComponent(const glm::vec4& color): Color(color) { }

		ColorComponent(const ColorComponent& color) = default;

		ColorComponent& operator=(ColorComponent& rhs) {
			Color = rhs.Color;
			return *this;
		}

		/// data ///
		glm::vec4 Color{1.0f};
	};

}