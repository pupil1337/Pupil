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
		bool HasComponent() const {
			return registry->has<T>(entity);
		}
		
		template<typename T>
		T& GetComponent() const {
			PP_CORE_ASSERT(HasComponent<T>(), "Entity does not has this component!");
			return registry->get<T>(entity);
		}
		
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			PP_CORE_ASSERT(!HasComponent<T>(), "Entity has this component!");
			T& component = registry->emplace<T>(entity, std::forward<Args>(args)...);
			
			return component;
		}

		template<typename T>
		void RemoveComponent() {
			PP_CORE_ASSERT(HasComponent<T>(), "Entity does not has this component!");
			registry->remove<T>(entity);
			// registry->remove_if_exists<T>(entity);
		}

		operator bool() const { return entity != entt::null; }
		operator uint32_t() const { return (uint32_t)entity; }
		operator entt::entity() const { return entity; }
		bool operator==(const Entity& rhs) { return this->entity == rhs.entity && this->registry == rhs.registry; }
		bool operator!=(const Entity& rhs) { return !(*this == rhs); }

	private:
		entt::entity entity{ entt::null };
		entt::registry* registry = nullptr;// todo-

	};

}