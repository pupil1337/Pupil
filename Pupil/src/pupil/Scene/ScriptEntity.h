#pragma once
#include "pupil/Core/Core.h"

#include <entt.hpp>

namespace Pupil {

	class ScriptEntity {
	public:
		ScriptEntity() = default;
		ScriptEntity(const ScriptEntity& scriptEntity) = default;
		ScriptEntity(entt::entity entity, entt::registry* registry): entity(entity), registry(registry) { }

		template<typename T>
		T& GetComponent() {
			return registry->get<T>(entity);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			return registry->emplace<T>(entity, std::forward<Args>(args)...);
		}

	private:
		entt::entity entity;
		entt::registry* registry = nullptr;
	};

}