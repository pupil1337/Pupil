#pragma once
#include "pupil/Core/Core.h"

#include <entt.hpp>
#include "pupil/Scene/Component.h"
#include "pupil/Scene/Entity.h"

namespace Pupil {

	class Scene {
	public:
		friend class ScenePanel;
		friend class SceneSerializer;

		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity");
		void DeleteEntity(const Entity& entity);

		void OnUpdate(TimeStep ts);

		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		entt::registry m_Registry;
		uint32_t m_width = 0, m_height = 0;

	};
	
}