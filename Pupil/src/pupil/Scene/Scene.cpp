#include "pppch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include "pupil/Renderer/Renderer2D.h"

namespace Pupil {


	Scene::Scene() {
		
	}

	Scene::~Scene() {

	}

	Entity Scene::CreateEnitty(const std::string& name) {
		TagComponent tag(name);
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TagComponent>(entity, tag);

		return { entity, &this->m_Registry };
	}

	void Scene::OnUpdate() {
		auto group = m_Registry.group<TransformComponent>(entt::get<ColorComponent>);
		for (auto entity : group) {
			auto& [transformComp, colorComp] = group.get<TransformComponent, ColorComponent>(entity);
			Renderer2D::DrawQuad(transformComp.Transform, colorComp.Color);
		}
	}

}