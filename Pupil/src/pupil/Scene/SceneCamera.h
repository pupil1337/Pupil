#pragma once
#include "pupil/Core/Core.h"

#include "pupil/Renderer/Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Pupil {

	class SceneCamera : public Camera {
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
		
		void SetViewportSize(uint32_t width, uint32_t height);
		void CalculateProjection();
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; CalculateProjection(); }
		ProjectionType GetProjectionType() { return m_ProjectionType; }
		float GetAspectRatio() { return m_AspectRatio; }
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

		// Perspective
		void SetPersepctive(float Fov, float Near, float Far);

		void  SetPerspectiveFOV (float Fov)  { m_PerspectiveFOV  = Fov ; CalculateProjection(); }
		void  SetPerspectiveNear(float Near) { m_PerspectiveNear = Near; CalculateProjection(); }
		void  SetPerspectiveFar (float Far)  { m_PerspcetiveFar  = Far ; CalculateProjection(); }
		float GetPerspectiveFOV () const { return m_PerspectiveFOV ; }
		float GetPerspectiveNear() const { return m_PerspectiveNear; }
		float GetPerspectiveFar () const { return m_PerspcetiveFar ; }

		// Orthographic
		void SetOrthographic(uint32_t size, float OrthoNear, float OrthoFar);
		
		void  SetOrthographicSize(float orthoSize) { m_OrthographicSize = orthoSize; CalculateProjection(); }
		void  SetOrthographicNear(float orthoNear) { m_OrthographicNear = orthoNear; CalculateProjection(); }
		void  SetOrthographicFar (float orthoFar ) { m_OrthographicFar  = orthoFar ; CalculateProjection(); }
		float GetOrthographicSize() const { return m_OrthographicSize; }
		float GetOrthographicNear() const { return m_OrthographicNear; }
		float GetOrthographicFar () const { return m_OrthographicFar ; }

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_AspectRatio = 16.0f/9.0f;

		// Perspective
		float m_PerspectiveFOV  = 45.0f;
		float m_PerspectiveNear = 0.01f;
		float m_PerspcetiveFar  = 1000.0f;

		// Orthographic
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar  = 1.0f;
	};

}