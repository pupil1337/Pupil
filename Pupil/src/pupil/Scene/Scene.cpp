#include "pppch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include "pupil/Renderer/Renderer2D.h"

namespace Pupil {


	Scene::Scene() {
		
	}

	Scene::~Scene() {

	}

	entt::entity Scene::CreateEnitty() {
		return m_Registry.create();
	}

	void Scene::OnUpdate() {
		auto group = m_Registry.group<TransformComponent>(entt::get<ColorComponent>);
		for (auto entity : group) {
			auto& [transformComp, colorComp] = group.get<TransformComponent, ColorComponent>(entity);
			Renderer2D::DrawQuad(transformComp.Transform, colorComp.Color);
		}
	}

}