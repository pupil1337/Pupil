#pragma once

#include "pupil/Core/Core.h"

#include "pupil/Scene/Scene.h"

namespace Pupil {

	class SceneSerializer {
	public:
		SceneSerializer(const Ref<Scene>& scene): m_Scene(scene) { }
		
		void Serialize(const std::string& filePath);
		bool DeSerialize(const std::string& filePath);

	private:
		Ref<Scene> m_Scene;

	};

}