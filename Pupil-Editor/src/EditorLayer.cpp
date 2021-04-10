#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "pupil/Utils/PlatformUtils.h"

namespace Pupil {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_OrthoCameraController(1280.0f / 720.0f, true), m_Color({ 0.8f, 0.2f, 0.8f, 1.0f }) {
		m_OrthoCameraController.m_Edit = true;
	}

	void EditorLayer::OnAttach() {
		PP_PROFILE_FUNCTION();

		m_Framebuffer = Framebuffer::Create({ 1280, 720 });

		// Scene
		m_Scene = std::make_shared<Scene>();
		m_ScenePanel.SetScenePanel(m_Scene);

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

		// ToDo
		/*glm::mat4 model = glm::translate(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f });
		PP_CORE_INFO("{0} {1} {2} {3}", model[0][0], model[0][1], model[0][2], model[0][3]);
		PP_CORE_INFO("{0} {1} {2} {3}", model[1][0], model[1][1], model[1][2], model[1][3]);
		PP_CORE_INFO("{0} {1} {2} {3}", model[2][0], model[2][1], model[2][2], model[2][3]);
		PP_CORE_INFO("{0} {1} {2} {3}", model[3][0], model[3][1], model[3][2], model[3][3]);
		glm::vec4 pos = { 1.0f, 1.0f, 0.0f, 1.0f };
		pos = model * pos;
		PP_CORE_WARN("{0} {1} {2} {3}", pos.x, pos.y, pos.z, pos.w);*/
	}

	void EditorLayer::OnUpdate(TimeStep ts) {
		PP_PROFILE_FUNCTION();

		m_TimeStep = ts;
		{
			PP_PROFILE_SCOPE("OrthoCameraController OnUpdate");

			if (m_ViewportFocused) {
				m_OrthoCameraController.OnUpdate(ts);
			}
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

		m_Scene->OnUpdate(ts);

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

		ImGui::End();

		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport");
			// viewport size change
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer().BlockEvent(!m_ViewportFocused || !m_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*)&viewportPanelSize) {
			m_ViewportSize = *(glm::vec2*)&viewportPanelSize;
			m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
			m_OrthoCameraController.SetAspectRatio(m_ViewportSize.x, m_ViewportSize.y);
			m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
		}

			// show texture
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, viewportPanelSize, { 0, 1 }, { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
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
		}

		return false;
	}

	void EditorLayer::OnEvent(Event& event) {
		PP_PROFILE_FUNCTION();

		m_OrthoCameraController.OnEvent(event);

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
			m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
			m_ScenePanel.SetScenePanel(m_Scene);

			SceneSerializer serializer(m_Scene);
			serializer.DeSerialize(filePath);
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