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

		void DrawEntityNode(entt::entity entity);

	private:
		Ref<Scene> m_Context;
		entt::entity m_EntitySelected;
	};

}