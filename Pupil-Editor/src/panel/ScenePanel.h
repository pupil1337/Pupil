#pragma once
#include "pupil/Core/Core.h"

#include "pupil/Scene/Scene.h"

namespace Pupil {

	class ScenePanel {
	public:
		ScenePanel() = default;
		ScenePanel(const Ref<Scene>& scene): m_Context(scene) { }

		void SetScenePanel(const Ref<Scene>& scene);

		void OnImGuiRender();

		void DrawEntityNode(const Entity& entity);
		void DrawComponents();

		Entity GetSelectedEntity() { return m_EntitySelected; }

	private:
		void DrawVec3Control(const std::string& label, glm::vec3& value, float revalue = 0.0f, float columnWidth = 70.0f);

	private:
		Ref<Scene> m_Context;
		Entity m_EntitySelected;
	};

}