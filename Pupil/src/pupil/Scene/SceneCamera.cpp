#include "pppch.h"
#include "SceneCamera.h"

namespace Pupil {
	
	SceneCamera::SceneCamera() {
		CalculateProjection();
	}

	void SceneCamera::CalculateProjection() {
		float left = -OrthoGraphicSize * OrthoAspectRatio * 0.5;
		float right = OrthoGraphicSize * OrthoAspectRatio * 0.5;
		float bottom = -OrthoGraphicSize * 0.5;
		float top = OrthoGraphicSize * 0.5;
		Projection = glm::ortho(left, right, bottom, top);
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
		OrthoAspectRatio = (float)width / (float)height;
		CalculateProjection();
	}

	void SceneCamera::SetOrthoGraphic(uint32_t size, float clipNear, float clipFar) {
		OrthoGraphicSize = size;
		OrthoNear = clipNear;
		OrthoFar = clipFar;
		CalculateProjection();
	}

}