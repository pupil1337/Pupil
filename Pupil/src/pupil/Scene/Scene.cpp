#include "pppch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include "pupil/Renderer/Renderer2D.h"
#include "pupil/Renderer/Camera.h"

namespace Pupil {


	Scene::Scene() {
		
	}

	Scene::~Scene() {

	}

	Entity Scene::CreateEntity(const std::string& name) {
		TagComponent tag(name);
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TagComponent>(entity, tag);

		return { entity, &this->m_Registry };
	}

	void Scene::OnUpdate(TimeStep ts) {

		// Script
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
			if (!nsc.Instance) {
				nsc.Instance = nsc.CreateNativeScriptComp(entity, &this->m_Registry);
			}
			nsc.Instance->OnUpdate(ts);
		});

		// GetCamera
		Camera* camera = nullptr;
		glm::mat4 transform = glm::mat4(1.0f);
		auto group1 = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
		for (auto entity : group1) {
			auto [cameraComp, transComp] = group1.get<CameraComponent, TransformComponent>(entity);
			if (cameraComp.Primary) {
				camera = &cameraComp.Camera;
				transform = transComp.Transform;
				break;
			}
		}

		if (camera) {
			Renderer2D::BeginScene(*camera, transform);
			auto group2 = m_Registry.group<TransformComponent>(entt::get<ColorComponent>);
			for (auto entity : group2) {
				auto [transformComp, colorComp] = group2.get<TransformComponent, ColorComponent>(entity);
				Renderer2D::DrawQuad(transformComp.Transform, colorComp.Color);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& sceneCameraComp = view.get<CameraComponent>(entity);
			if (sceneCameraComp.FixAspectRatio) {
				sceneCameraComp.Camera.SetViewportSize(width, height);
			}
		}
	}

}