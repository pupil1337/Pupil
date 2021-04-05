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
		void DrawComponent();

	private:
		Ref<Scene> m_Context;
		Entity m_EntitySelected;
	};

}