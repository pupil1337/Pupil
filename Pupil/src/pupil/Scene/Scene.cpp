#include "pppch.h"
#include "Scene.h"

#include <glm/glm.hpp>

namespace Pupil {


	Scene::Scene() {

		struct TransformComponent {
			TransformComponent() = default;
			TransformComponent(const TransformComponent& trans) = default;
			TransformComponent(const glm::mat4& trans) : Transform(trans) { }
			
			TransformComponent& operator=(const TransformComponent& rhs) {
				Transform = rhs.Transform;
				return *this;
			}

			operator glm::mat4& () { return Transform; }
			operator const glm::mat4& () const { return Transform; }

			glm::mat4 Transform;
		};

		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
		// ----- to be continue
	}

	Scene::~Scene() {

	}

}