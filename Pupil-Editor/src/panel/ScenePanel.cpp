#include "pppch.h"
#include "ScenePanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Pupil {

	void Pupil::ScenePanel::SetScenePanel(const Ref<Scene>& scene) {
		m_Context = scene;
		m_EntitySelected = {};
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

		// Right-click on space-filed of ScenePanel
		if (ImGui::BeginPopupContextWindow(nullptr, 1, false)) {
			if (ImGui::MenuItem("Create a Entity")) {
				m_Context->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		// Property
		ImGui::Begin("Property");

		if (m_EntitySelected) {
			DrawComponents();
		}

		ImGui::End();
	}

	void ScenePanel::DrawEntityNode(const Entity& entity) {
		auto& tag = entity.GetComponent<TagComponent>();

		ImGuiTreeNodeFlags flags = (m_EntitySelected == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.Name.c_str());
		if (ImGui::IsItemClicked()) {
			m_EntitySelected = entity;
		}
		
		if (opened) {
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)((uint32_t)entity + 100), flags, tag.Name.c_str());
			if (opened) {
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::PushID((void*)(uint32_t)entity);
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete this Entity")) {
				m_Context->DeleteEntity(entity);
				if(m_EntitySelected == entity) m_EntitySelected = {};
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();

	}

	void ScenePanel::DrawVec3Control(const std::string& label, glm::vec3& value, float revalue, float columnWidth) {
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.8f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X")) value.x = revalue;
		ImGui::PopFont();
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.x, 1.0f, 0.0f, 0.0f, "%0.2f");
		ImGui::PopItemWidth();
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
		ImGui::SameLine();
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y")) value.y = revalue;
		ImGui::PopFont();
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.y, 1.0f, 0.0f, 0.0f, "%0.2f");
		ImGui::PopItemWidth();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));
		ImGui::SameLine();
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z")) value.z = revalue;
		ImGui::PopFont();
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.z, 1.0f, 0.0f, 0.0f, "%0.2f");
		ImGui::PopItemWidth();
		
		ImGui::Columns(1);

		ImGui::PopStyleVar();
		ImGui::PopID();
	}

	template<typename T, typename UiFunction>
	static void DrawComponet(const std::string& name, Entity entity, UiFunction uiFunc) {
		auto& component = entity.GetComponent<T>();

		constexpr ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		ImGui::Separator();
		bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::PopStyleVar();
		
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight})) {
			ImGui::OpenPopup("Edit Component");
		}
		bool deleted = false;
		if (ImGui::BeginPopup("Edit Component")) {
			if (ImGui::MenuItem("Delete this Component")) {
				deleted = true;

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (opened) {
			uiFunc(component);
			ImGui::TreePop();
		}

		if (deleted) entity.RemoveComponent<T>();
	}

	void ScenePanel::DrawComponents() {
		// TagComponent
		if (m_EntitySelected.HasComponent<TagComponent>()) {
			auto& tag = m_EntitySelected.GetComponent<TagComponent>();

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.Name.c_str());
			if (ImGui::InputText("##tag", buffer, sizeof(buffer))) {
				tag.Name = std::string(buffer);
			}
		}

		ImGui::PushItemWidth(-1);
		ImGui::SameLine();
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("Add Component");
		}
		if (ImGui::BeginPopup("Add Component")) {
			if (ImGui::MenuItem("Transform")) {
				m_EntitySelected.AddComponent<TransformComponent>();

				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Color")) {
				m_EntitySelected.AddComponent<ColorComponent>();

				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Camera")) {
				m_EntitySelected.AddComponent<CameraComponent>();
				m_Context->OnViewportResize(m_Context->m_width, m_Context->m_height);

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		// TransformComponent
		if (m_EntitySelected.HasComponent<TransformComponent>()) {
			DrawComponet<TransformComponent>("Transform", m_EntitySelected, [&](auto& component) {
				DrawVec3Control("position", component.translation);
				DrawVec3Control("rotate", component.rotation);
				DrawVec3Control("scale", component.scale, 1.0f);
			});
		}

		// CameraComponent
		if (m_EntitySelected.HasComponent<CameraComponent>()) {
			DrawComponet<CameraComponent>("Camera", m_EntitySelected, [&](auto& component) {
				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionType[] = { "Perspective", "Orthographic" };
				const char* currentProjectionType = projectionType[(int)component.Camera.GetProjectionType()];

				if (ImGui::BeginCombo("projection", currentProjectionType)) {
					for (int i = 0; i != 2; ++i) {
						bool isSelected = projectionType[i] == currentProjectionType;
						if (ImGui::Selectable(projectionType[i], isSelected)) {

							currentProjectionType = projectionType[i];
							component.Camera.SetProjectionType((SceneCamera::ProjectionType)i);

						}
					}

					ImGui::EndCombo();
				}

				if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
					float Fov = component.Camera.GetPerspectiveFOV();
					if (ImGui::DragFloat("fov", &Fov, 0.5f, 10.0f, 90.0f, "%.1f")) {
						component.Camera.SetPerspectiveFOV(Fov);
					}
					float Near = component.Camera.GetPerspectiveNear();
					if (ImGui::DragFloat("near", &Near, 0.005f, 0.01f, 1.0f)) {
						component.Camera.SetPerspectiveNear(Near);
					}
					float Far = component.Camera.GetPerspectiveFar();
					if (ImGui::DragFloat("far", &Far, 10.0f, 100.0f, 10000.0f, "%.0f")) {
						component.Camera.SetPerspectiveFar(Far);
					}
				}

				if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
					float Size = component.Camera.GetOrthographicSize();
					if (ImGui::DragFloat("size", &Size, 0.02f, 0.1f, 100.0f, "%.2f")) {
						component.Camera.SetOrthographicSize(Size);
					}
					float Near = component.Camera.GetOrthographicNear();
					if (ImGui::DragFloat("near", &Near, 0.005f, 0.01f, 1.0f)) {
						component.Camera.SetOrthographicNear(Near);
					}
					float Far = component.Camera.GetOrthographicFar();
					if (ImGui::DragFloat("far", &Far, 10.0f, 100.0f, 10000.0f, "%.0f")) {
						component.Camera.SetOrthographicFar(Far);
					}
				}
			});
		}

		// ColorComponent
		if (m_EntitySelected.HasComponent<ColorComponent>()) {
			DrawComponet<ColorComponent>("Color", m_EntitySelected, [&](auto& component) {
				ImGui::ColorEdit4("##color", glm::value_ptr(component.Color));
			});
		}
	}

}