#pragma once
#include "pupil/Core/Core.h"

#include "pupil/Renderer/Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Pupil {

	class SceneCamera : public Camera {
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
		
		void CalculateProjection();

		void SetViewportSize(uint32_t width, uint32_t height);
		void SetOrthoGraphic(uint32_t size, float OrthoNear, float OrthoFar);
		// todo orthoSize < 0
		void SetOrthoGraphicSize(float orthoSize) { OrthoGraphicSize = orthoSize; CalculateProjection(); }
		float GetOrthoGraphicSize() const { return OrthoGraphicSize; }

	private:
		float OrthoGraphicSize = 10.0f;
		float OrthoNear = -1.0f;
		float OrthoFar  = 1.0f;
		float OrthoAspectRatio= 16.0f / 9.0f;
	};

}