#pragma once
#include "pupil/Core/Core.h"

#include <entt.hpp>
#include "pupil/Scene/Component.h"

namespace Pupil {

	class Scene {
	public:
		Scene();
		~Scene();

		entt::entity CreateEnitty();

		// Temp
		inline entt::registry& GetReg() { return m_Registry; }

		void OnUpdate();

	private:
		entt::registry m_Registry;

	};
	
}