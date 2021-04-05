#include "pppch.h"
#include "ScenePanel.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Pupil {

	void Pupil::ScenePanel::SetScenePanel(const Ref<Scene>& scene) {
		m_Context = scene;
	}

	void Pupil::ScenePanel::OnImGuiRender() {
		// ScenePanel
		ImGui::Begin("ScenePanel");

		m_Context->m_Registry.each([&](auto entityID) {
			Entity entity{ entityID, &m_Context.get()->m_Registry };
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_EntitySelected = {};
		}

		ImGui::End();

		// Property
		ImGui::Begin("Property");

		if(m_EntitySelected) {
			DrawComponent();
		}

		ImGui::End();
	}

	void ScenePanel::DrawEntityNode(const Entity& entity) {
		auto& tag = entity.GetComponent<TagComponent>();
		
		ImGuiTreeNodeFlags flags = (m_EntitySelected == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.Name.c_str());
		if (ImGui::IsItemClicked()) {
			m_EntitySelected = entity;
		}
		if (opened) {
			ImGuiTreeNodeFlags flags = (m_EntitySelected == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)((uint32_t)entity + 100), flags, tag.Name.c_str());
			if (opened) {
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}

	void ScenePanel::DrawComponent() {
		if (m_EntitySelected.HasComponent<TagComponent>()) {
			auto& tag = m_EntitySelected.GetComponent<TagComponent>();
			
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.Name.c_str());
			if (ImGui::InputText("tag", buffer, sizeof(buffer))) {
				tag.Name = std::string(buffer);
			}
		}

		if (m_EntitySelected.HasComponent<TransformComponent>()) {
			auto& transform = m_EntitySelected.GetComponent<TransformComponent>();
			
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "transform")) {
				ImGui::DragFloat3("position", glm::value_ptr(transform.Transform[3]));
				ImGui::TreePop();
			}

		}
	}

}