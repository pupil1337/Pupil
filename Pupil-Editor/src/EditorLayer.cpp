#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>

#include "pupil/Utils/PlatformUtils.h"

#include "pupil/Math/Math.h"


namespace Pupil {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_OrthoCameraController(1280.0f / 720.0f, true) {
		m_OrthoCameraController.m_Edit = true;
	}

	void EditorLayer::OnAttach() {
		PP_PROFILE_FUNCTION();

		FramebufferSpecification spc;
		spc.Width = 1280, spc.Height = 720;
		spc.Attachments = { FramebufferTextureFormt::RGBA, FramebufferTextureFormt::RGBA, FramebufferTextureFormt::DEPTH32STENCIL8 };
		m_Framebuffer = Framebuffer::Create(spc);

		// Scene
		m_Scene = std::make_shared<Scene>();
		m_ScenePanel.SetScenePanel(m_Scene);
		m_EditorCamera = EditorCamera(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
#if 0
		// Red Entity
		m_RedEntity = m_Scene->CreateEntity("RedEntity");
		m_RedEntity.AddComponent<TransformComponent>(glm::vec3(0.0f));
		m_RedEntity.AddComponent<ColorComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		// Green Entity
		m_GreenEntity = m_Scene->CreateEntity("GreenEntity");
		m_GreenEntity.AddComponent<TransformComponent>(glm::vec3(0.0f));
		m_GreenEntity.AddComponent<ColorComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		// Camera Entity
		m_Camera = m_Scene->CreateEntity("Camera");
		m_Camera.AddComponent<CameraComponent>();
		m_Camera.AddComponent<TransformComponent>(glm::vec3(0.0f));

		// NativeScript...  todo C# Script
		class ScriptCamera : public ScriptEntity {
		public:
			virtual void OnCreate() override {
				PP_CORE_TRACE("ScriptCamera Create");
			}

			virtual void OnDestroy() override {
				
			}

			virtual void OnUpdate(TimeStep ts) override {
				auto& transform = GetComponent<TransformComponent>().translation;
				float speed = 5.0f;
				if (Input::IsKeyPressed(PP_KEY_A)) transform.x -= speed * ts;
				if (Input::IsKeyPressed(PP_KEY_D)) transform.x += speed * ts;
				if (Input::IsKeyPressed(PP_KEY_S)) transform.y -= speed * ts;
				if (Input::IsKeyPressed(PP_KEY_W)) transform.y += speed * ts;
			}
		};

		m_Camera.AddComponent<NativeScriptComponent>().Bind<ScriptCamera>();

		// Clip-space Camera
		m_ClipCamera = m_Scene->CreateEntity("Clip-space Camera");
		m_ClipCamera.AddComponent<CameraComponent>().Primary = false;
		m_ClipCamera.AddComponent<TransformComponent>(glm::vec3(0.0f));
		m_ClipCamera.AddComponent<NativeScriptComponent>().Bind<ScriptCamera>();
#endif
	}

	void EditorLayer::OnUpdate(TimeStep ts) {
		PP_PROFILE_FUNCTION();

		m_TimeStep = ts;
		{
			PP_PROFILE_SCOPE("OrthoCameraController OnUpdate");

			if (m_ViewportFocused) {
				m_OrthoCameraController.OnUpdate(ts);
			}
			m_EditorCamera.OnUpdate(ts);
		}

		{ 
			Renderer2D::ResetStats();
			m_Framebuffer->Bind(); 
		}

		{
			PP_PROFILE_SCOPE("Renderer::Prep");

			RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			RenderCommand::Clear();
		}  

		m_Scene->OnUpdateEditor(ts, m_EditorCamera);
		    
		m_Framebuffer->UnBind();
	}

	void EditorLayer::OnImGuiRender() {
		PP_PROFILE_FUNCTION();

		/// dockspace  ///
		static bool opt_fullscreen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", (bool*)true, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 330.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;
		//else ShowDockingDisabledMessage();

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {

				if (ImGui::MenuItem("New", "Ctrl + N")) NewScene();
				if (ImGui::MenuItem("Open...", "Ctrl + O")) OpenScene();
				if (ImGui::MenuItem("Save...", "Ctrl + S")) SaveScene();
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		///////////////////////////////////////////////////////
		//////               Windows                     //////
		///////////////////////////////////////////////////////

		// ScenePanel
		m_ScenePanel.OnImGuiRender();

		// Settings
		ImGui::Begin("Settings");
			// render stats
		auto stats = Pupil::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads:      %d", stats.QuadCounts);
		ImGui::Text("Vertices:   %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices:    %d", stats.GetTotalIndexCount());
			// performance plotLines
		ImGui::Text("Performance:");
		m_FrameTimeGraph[values_offset] = m_TimeStep.GetMilliSecond();
		values_offset = (values_offset + 1) % 100;
		ImGui::PlotLines("#FrameTime", m_FrameTimeGraph, 100, values_offset, "FrameTime (ms)", 0.0f, 200.0f, ImVec2(0, 100), 100);
		ImGui::Text("FrameTime: %.2f (Fps: %d)", m_TimeStep.GetMilliSecond(), (int)(1.0f / m_TimeStep.GetSecond()));

		ImGui::End();// end Settings

		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");
			// viewport size change
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer().BlockEvent(!m_ViewportFocused && !m_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*)&viewportPanelSize) {
			m_ViewportSize = *(glm::vec2*)&viewportPanelSize;
			m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
			m_OrthoCameraController.SetAspectRatio(m_ViewportSize.x, m_ViewportSize.y);
			m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

			// show texture
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(1);
		ImGui::Image((void*)textureID, viewportPanelSize, { 0, 1 }, { 1, 0 });
		
			// Gizmos 必须在要画的framebuffer里面。
		Entity selectedEntity = m_ScenePanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1 && selectedEntity.HasComponent<TransformComponent>()) {
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = ImGui::GetWindowWidth();
			float windowHeight = ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera
			//auto cameraEntity = m_Scene->GetPrimaryCamera();
			//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			//const glm::mat4& cameraProjection = camera.GetProjection();
			//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor Camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(PP_KEY_LEFT_CONTROL);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) snapValue = 45.0f;
			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr,
				snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.rotation;
				tc.translation = translation;
				tc.rotation = rotation;
				tc.scale = scale;
			}

		}
		ImGui::End();// end Viewport
		ImGui::PopStyleVar();

		ImGui::End();// end docking
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {
		if (e.GetRepeatCount() > 0) return false;

		bool control = Input::IsKeyPressed(PP_KEY_LEFT_CONTROL) | Input::IsKeyPressed(PP_KEY_RIGHT_CONTROL);
		
		switch (e.GetKeyCode()) {
			case PP_KEY_N: {
				if (control) NewScene();
				break;
			}
			case PP_KEY_O: {
				if (control) OpenScene();
				break;
			}
			case PP_KEY_S: {
				if (control) SaveScene();
				break;
			}
			// Gizmos
			case PP_KEY_Q: {
				if (!ImGuizmo::IsUsing()) m_GizmoType = -1;
				break;
			}
			case PP_KEY_W: {
				if (!ImGuizmo::IsUsing()) m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case PP_KEY_E: {
				if (!ImGuizmo::IsUsing()) m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case PP_KEY_R: {
				if (!ImGuizmo::IsUsing()) m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}

		return false;
	}

	void EditorLayer::OnEvent(Event& event) {
		PP_PROFILE_FUNCTION();

		m_OrthoCameraController.OnEvent(event);
		m_EditorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(PP_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	void EditorLayer::NewScene() {
		m_Scene = std::make_shared<Scene>();
		m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
		m_ScenePanel.SetScenePanel(m_Scene);
	}

	void EditorLayer::OpenScene() {
		std::string filePath = FileDialog::OpenFile("Pupil Scene (*.yaml)\0*.yaml\0");
		if (!filePath.empty()) {
			m_Scene = std::make_shared<Scene>();
			SceneSerializer serializer(m_Scene);
			serializer.DeSerialize(filePath);

			m_ScenePanel.SetScenePanel(m_Scene);
			m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
		}
	}

	void EditorLayer::SaveScene() {
		std::string filePath = FileDialog::SaveFile("Pupil Scene (*.yaml)\0*.yaml\0");
		if (!filePath.empty()) {

			SceneSerializer serializer(m_Scene);
			serializer.Serialize(filePath);
		}
	}

	void EditorLayer::OnDetach() {
		PP_PROFILE_FUNCTION();

	}

}