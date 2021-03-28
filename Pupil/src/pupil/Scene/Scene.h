#pragma once
#include "pupil/Core/Core.h"

#include "entt.hpp"

namespace Pupil {

	class Scene {
	public:
		Scene();
		~Scene();

	private:
		entt::registry m_Registry;

	};
	
}