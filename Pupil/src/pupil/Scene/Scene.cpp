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

	void Scene::DeleteEntity(const Entity& entity) {
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateRuntime(TimeStep ts) {
		if (!m_Registry.empty()) {
			// Script
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
				if (!nsc.Instance) {
					nsc.Instance = nsc.CreateNativeScriptComp();
					nsc.Instance->entity = entity;
					nsc.Instance->registry = &m_Registry;
					nsc.Instance->OnCreate();
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
					transform = transComp.GetTransform();
					break;
				}
			}

			if (camera) {
				Renderer2D::BeginScene(*camera, transform);
				auto group2 = m_Registry.group<TransformComponent>(entt::get<ColorComponent>);
				for (auto entity : group2) {
					auto [transformComp, colorComp] = group2.get<TransformComponent, ColorComponent>(entity);
					Renderer2D::DrawQuad(transformComp.GetTransform(), colorComp.Color);
				}
				Renderer2D::EndScene();
			}
		}
	}

	void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& editorCamera) {
		if (!m_Registry.empty()) {
			Renderer2D::BeginScene(editorCamera);

			auto group = m_Registry.group<TransformComponent>(entt::get<ColorComponent>);
			for (auto entity : group) {
				auto [transformComp, colorComp] = group.get<TransformComponent, ColorComponent>(entity);
				Renderer2D::DrawQuad(transformComp.GetTransform(), colorComp.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		m_width = width, m_height = height;
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& sceneCameraComp = view.get<CameraComponent>(entity);
			if (sceneCameraComp.FixAspectRatio) {
				sceneCameraComp.Camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::GetPrimaryCamera() {
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary) return Entity{ entity, &this->m_Registry };
		}
		return Entity{ };
	}

}