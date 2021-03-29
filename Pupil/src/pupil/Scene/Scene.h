#pragma once
#include "pupil/Core/Core.h"

#include <entt.hpp>
#include "pupil/Scene/Component.h"
#include "pupil/Scene/Entity.h"

namespace Pupil {

	class Scene {
	public:
		Scene();
		~Scene();

		Entity CreateEnitty(const std::string& name = "Entity");

		void OnUpdate();

	private:
		entt::registry m_Registry;

	};
	
}