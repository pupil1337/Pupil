#include "pppch.h"
#include "ScenePanel.h"

#include <imgui/imgui.h>

namespace Pupil {

	void Pupil::ScenePanel::SetScenePanel(const Ref<Scene>& scene) {
		m_Context = scene;
	}

	void Pupil::ScenePanel::OnImGuiRender() {
		ImGui::Begin("ScenePanel");

		m_Context->m_Registry.each([&](auto entity) {
			DrawEntityNode(entity);
		});

		ImGui::End();
	}

	void ScenePanel::DrawEntityNode(entt::entity entity) {
		auto& tag = m_Context->m_Registry.get<TagComponent>(entity);
		
		ImGuiTreeNodeFlags flags = (m_EntitySelected == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.Name.c_str());
		if (ImGui::IsItemClicked()) {
			m_EntitySelected = entity;
		}
		if (opened) {
			ImGuiTreeNodeFlags flags = (m_EntitySelected == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)((uint32_t)entity+100), flags, tag.Name.c_str());
			if (opened) {
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}

}