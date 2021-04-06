#include "pppch.h"
#include "ScenePanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
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

		if (m_EntitySelected) {
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

	void DrawVec3Control(const std::string& label, glm::vec3& value, float revalue = 0.0f, float columnWidth = 70.0f) {
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.8f));
		if (ImGui::Button("X")) {
			value.x = revalue;
		}
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.x, 0.1f, 0.0f, 0.0f, "%0.2f");
		ImGui::SameLine();
		ImGui::PopItemWidth();
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
		if (ImGui::Button("Y")) {
			value.y = revalue;
		}
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.y, 0.1f, 0.0f, 0.0f, "%0.2f");
		ImGui::SameLine();
		ImGui::PopItemWidth();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));
		if (ImGui::Button("Z")) {
			value.z = revalue;
		}
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.z, 0.1f, 0.0f, 0.0f, "%0.2f");
		ImGui::SameLine();
		ImGui::PopItemWidth();
		
		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void ScenePanel::DrawComponent() {
		// TagComponent
		if (m_EntitySelected.HasComponent<TagComponent>()) {
			auto& tag = m_EntitySelected.GetComponent<TagComponent>();

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.Name.c_str());
			if (ImGui::InputText("tag", buffer, sizeof(buffer))) {
				tag.Name = std::string(buffer);
			}
		}

		// TransformComponent
		if (m_EntitySelected.HasComponent<TransformComponent>()) {
			auto& transform = m_EntitySelected.GetComponent<TransformComponent>();

			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				//ImGui::DragFloat3("position", glm::value_ptr(transform.translation), 0.1f);
				DrawVec3Control("position", transform.translation);
				DrawVec3Control("rotate", transform.rotation);
				DrawVec3Control("scale", transform.scale, 1.0f);
				ImGui::TreePop();
			}

		}

		// CameraComponent
		if (m_EntitySelected.HasComponent<CameraComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
				auto& camera = m_EntitySelected.GetComponent<CameraComponent>();

				ImGui::Checkbox("Primary", &camera.Primary);

				const char* projectionType[] = { "Perspective", "Orthographic" };
				const char* currentProjectionType = projectionType[(int)camera.Camera.GetProjectionType()];

				if (ImGui::BeginCombo("projection", currentProjectionType)) {
					for (int i = 0; i != 2; ++i) {
						bool isSelected = projectionType[i] == currentProjectionType;
						if (ImGui::Selectable(projectionType[i], isSelected)) {

							currentProjectionType = projectionType[i];
							camera.Camera.SetProjectionType((SceneCamera::ProjectionType)i);

						}
						// if (isSelected) {
						// 	ImGui::SetItemDefaultFocus();
						// } // in is maybe do nothing
					}

					ImGui::EndCombo();
				}

				if (camera.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
					float Fov = camera.Camera.GetPerspectiveFOV();
					if (ImGui::DragFloat("fov", &Fov, 0.5f, 10.0f, 90.0f, "%.1f")) {
						camera.Camera.SetPerspectiveFOV(Fov);
					}
					float Near = camera.Camera.GetPerspectiveNear();
					if (ImGui::DragFloat("near", &Near, 0.005f, 0.01f, 1.0f)) {
						camera.Camera.SetPerspectiveNear(Near);
					}
					float Far = camera.Camera.GetPerspectiveFar();
					if (ImGui::DragFloat("far", &Far, 10.0f, 100.0f, 10000.0f, "%.0f")) {
						camera.Camera.SetPerspectiveFar(Far);
					}
				}

				if (camera.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
					float Size = camera.Camera.GetOrthographicSize();
					if (ImGui::DragFloat("size", &Size, 0.02f, 0.1f, 100.0f, "%.2f")) {
						camera.Camera.SetOrthographicSize(Size);
					}
					float Near = camera.Camera.GetOrthographicNear();
					if (ImGui::DragFloat("near", &Near, 0.005f, 0.01f, 1.0f)) {
						camera.Camera.SetOrthographicNear(Near);
					}
					float Far = camera.Camera.GetOrthographicFar();
					if (ImGui::DragFloat("far", &Far, 10.0f, 100.0f, 10000.0f, "%.0f")) {
						camera.Camera.SetOrthographicFar(Far);
					}
				}

				ImGui::TreePop();
			}
		}

		// ColorComponent
		if (m_EntitySelected.HasComponent<ColorComponent>()) {
			if (ImGui::TreeNodeEx((void*)typeid(ColorComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Color")) {
				auto& color = m_EntitySelected.GetComponent<ColorComponent>();

				ImGui::ColorEdit4("##color", glm::value_ptr(color.Color));

				ImGui::TreePop();
			}
		}
	}

}