#pragma once
#include "pupil/Core/core.h"

#include <entt.hpp>

namespace Pupil {
	
	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity entity, entt::registry* registry): entity(entity), registry(registry) { }
		Entity(const Entity& other) = default;

		template<typename T>
		bool HasComponent() {
			return registry->has<T>(entity);
		}
		
		template<typename T>
		T& GetComponent() {
			PP_CORE_ASSERT(HasComponent<T>(), "Entity does not has this component!");
			return registry->get<T>(entity);
		}
		
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			PP_CORE_ASSERT(!HasComponent<T>(), "Entity has this component!");
			return registry->emplace<T>(entity, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent() {
			PP_CORE_ASSERT(HasComponent<T>(), "Entity does not has this component!");
			registry->remove<T>(entity);
			// registry->remove_if_exists<T>(entity);
		}

	private:
		entt::entity entity;
		entt::registry* registry;
	};

}