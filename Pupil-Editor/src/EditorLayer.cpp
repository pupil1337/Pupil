#include "pppch.h"
#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define Editor 1

const uint32_t m_MapWidth = 24;
const uint32_t m_MapHeight = 10;
const char* m_MapTiles =
"WWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWDDDDDWWWWWWWWWW"
"WWWWWWWDDDDDDDDDWWWWWWWW"
"WWWWWWDDDDDDDDDDDDWWWTWW"
"WWWWWDDDDDDDDDDDDDDWWWWW"
"WWWWDDDDDDDDDDDDDDWWWWWW"
"WWWWWDDDDDDDDDDDDDDWWWWW"
"WW6WWWDDDDDDDDDDDDWWWWWW"
"WWWWWWDDDDDDDDDDDWWWWWWW"
"WWWWWWWWDDDDDDDWWWWWWWWW";

namespace Pupil {

	EditorLayer::EditorLayer()
	: Layer("EditorLayer"), m_OrthoCameraController(1280.0f / 720.0f, true), m_Color({ 0.8f, 0.2f, 0.8f, 1.0f }) {
		m_OrthoCameraController.m_Edit = true;
	}

	void EditorLayer::OnAttach() {
		PP_PROFILE_FUNCTION();

		m_Texture1 = Texture2D::Create("assets/textures/awesomeface.png");
		m_Texture2 = Texture2D::Create("assets/textures/container2.png");
		m_Texture3 = Texture2D::Create("assets/textures/checkerboard.png");

		m_SpriteSheet = Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
		m_SoilTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 }, { 1, 1 });
		m_WaterTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 }, { 1, 1 });
		m_TreeTexture   = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 1 }, { 128, 128 }, { 1, 2 });
		m_SpriteMap['W'] = m_SoilTexture;
		m_SpriteMap['D'] = m_WaterTexture;
		m_SpriteMap['T'] = m_TreeTexture;

		FramebufferSpecification fbspec;
		fbspec.Width = 1280;
		fbspec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbspec);

		m_Scene = std::make_shared<Scene>();

		m_SquareEntity = m_Scene->CreateEnitty("SquareEntity");
		m_SquareEntity.AddComponent<TransformComponent>(glm::mat4(1.0f));
		m_SquareEntity.AddComponent<ColorComponent>(glm::vec4(1.0f));

		// Init here
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 1.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };

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

		{
			PP_PROFILE_SCOPE("ParticleSystem");
			if (Input::IsMousePressed(PP_MOUSE_BUTTON_LEFT)) {
				auto [x, y] = Input::GetMousePosition();
				auto width = Application::Get().GetWindow().GetWidth();
				auto height = Application::Get().GetWindow().GetHeight();

				auto bounds = m_OrthoCameraController.GetBounds();
				auto pos = m_OrthoCameraController.GetCamera().GetPosition();
				m_Particle.Position.x = (pos.x - bounds.GetWidth() * 0.5) + (x / width) * bounds.GetWidth();
				m_Particle.Position.y = (pos.y + bounds.GetHeight() * 0.5) - (y / height) * bounds.GetHeight();
				for (int i = 0; i < 8; i++)
					m_ParticleSystem.Emit(m_Particle);
			}

			m_ParticleSystem.OnUpdate(ts);
			m_ParticleSystem.OnRender(m_OrthoCameraController.GetCamera());
		}

		Renderer2D::BeginScene(m_OrthoCameraController.GetCamera());
		m_Scene->OnUpdate();
		Renderer2D::EndScene();

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
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		//else ShowDockingDisabledMessage();

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

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
			// color edit
			ColorComponent& colorComp = m_SquareEntity.GetComponent<ColorComponent>();
			ImGui::ColorEdit4("Color", glm::value_ptr(colorComp.Color));
			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
			ImGui::Begin("Viewport");
			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer().BlockEvent(!m_ViewportFocused || !m_ViewportHovered);
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			//PP_CORE_INFO("viewportPlanelSize [{0}, {1}]", viewportPanelSize.x, viewportPanelSize.y);
			//PP_CORE_INFO("Camera.bounds [{0},{1},{2},{3}]", m_OrthoCameraController.GetBounds().Left, m_OrthoCameraController.GetBounds().Right, m_OrthoCameraController.GetBounds().Bottom, m_OrthoCameraController.GetBounds().Top);
			if (m_ViewportSize != *(glm::vec2*)&viewportPanelSize) {
				m_ViewportSize = *(glm::vec2*)&viewportPanelSize;
				m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
				m_OrthoCameraController.SetAspectRatio(m_ViewportSize.x, m_ViewportSize.y);
			}
			// show texture
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, viewportPanelSize, { 0, 1 }, { 1, 0 });
			ImGui::End();
			ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event) {
		PP_PROFILE_FUNCTION();

		m_OrthoCameraController.OnEvent(event);
	}
	
	void EditorLayer::OnDetach() {
		PP_PROFILE_FUNCTION();

	}

}