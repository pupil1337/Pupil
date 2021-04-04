#pragma once
#include "pupil/Core/Core.h"
#include "pupil/Core/TimeStep.h"

#include <entt.hpp>
#include "pupil/Scene/Scene.h"

namespace Pupil {

	class ScriptEntity {
	public:
		friend class Scene;

		ScriptEntity() = default;
		ScriptEntity(const ScriptEntity& scriptEntity) = default;
		ScriptEntity(entt::entity entity, entt::registry* registry): entity(entity), registry(registry) { }
		virtual ~ScriptEntity() = default;

		template<typename T>
		T& GetComponent() {
			return registry->get<T>(entity);
		}

		virtual void OnCreate() { }
		virtual void OnDestroy() { }
		virtual void OnUpdate(TimeStep ts) { }

	private:
		entt::entity entity{ entt::null };
		entt::registry* registry = nullptr;

	};

}