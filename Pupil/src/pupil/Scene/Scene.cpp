#include "pppch.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include "pupil/Renderer/Renderer2D.h"
#include "pupil/Renderer/Camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
					Renderer2D::DrawQuad((int)entity, transformComp.GetTransform(), colorComp.Color);
				}
				Renderer2D::EndScene();
			}
		}
	}

	void Scene::OnUpdateEditor(Entity selectedEntity, EditorCamera& editorCamera) {
		if (!m_Registry.empty()) {
			Renderer2D::BeginScene(editorCamera);
			glStencilMask(0x00);
			auto group = m_Registry.group<TransformComponent>(entt::get<ColorComponent>);
			for (auto entity : group) {
				if (selectedEntity == entity) continue;
				auto [transformComp, colorComp] = group.get<TransformComponent, ColorComponent>(entity);
				Renderer2D::DrawQuad((int)entity, transformComp.GetTransform(), colorComp.Color);
			}
			Renderer2D::EndScene();
			
			// ToDo fix
			if (selectedEntity && selectedEntity.HasComponent<TransformComponent>() && selectedEntity.HasComponent<ColorComponent>()) {
				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				auto& cc = selectedEntity.GetComponent<ColorComponent>();
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);
				Renderer2D::BeginScene(editorCamera);
				Renderer2D::DrawQuad((int)(uint32_t)selectedEntity, tc.GetTransform(), cc.Color);
				Renderer2D::EndScene();
				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);
				glDisable(GL_DEPTH_TEST);
				tc.scale.x *= 1.08f, tc.scale.y *= 1.08f, tc.scale.z *= 1.08f;
				Renderer2D::DrawLinear(tc.GetTransform(), editorCamera);
				tc.scale.x /= 1.08f, tc.scale.y /= 1.08f, tc.scale.z /= 1.08f;
				glStencilMask(0xFF);
				glStencilFunc(GL_ALWAYS, 0, 0xFF);
				glEnable(GL_DEPTH_TEST);
			}
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