#include "pppch.h"
#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_OrthoCameraController(1280.0f / 720.0f, true), m_Color({ 0.8f, 0.2f, 0.8f, 1.0f }) {

	}

	void Sandbox2D::OnAttach() {
		PP_PROFILE_FUNCTION();

		m_Texture1 = Pupil::Texture2D::Create("assets/textures/awesomeface.png");
		m_Texture2 = Pupil::Texture2D::Create("assets/textures/container2.png");
		m_Texture3 = Pupil::Texture2D::Create("assets/textures/checkerboard.png");

		m_SpriteSheet = Pupil::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
		m_SoilTexture = Pupil::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 }, { 1, 1 });
		m_WaterTexture = Pupil::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11, 11 }, { 128, 128 }, { 1, 1 });
		m_TreeTexture   = Pupil::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 1 }, { 128, 128 }, { 1, 2 });
		m_SpriteMap['W'] = m_SoilTexture;
		m_SpriteMap['D'] = m_WaterTexture;
		m_SpriteMap['T'] = m_TreeTexture;

		Pupil::FramebufferSpecification fbspec;
		fbspec.Width = 1280;
		fbspec.Height = 720;
		m_Framebuffer = Pupil::Framebuffer::Create(fbspec);

		// Init here
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 1.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };

	}

	void Sandbox2D::OnUpdate(Pupil::TimeStep ts) {
		PP_PROFILE_FUNCTION();
		
		m_TimeStep = ts;
		{
			PP_PROFILE_SCOPE("OrthoCameraController OnUpdate");

			m_OrthoCameraController.OnUpdate(ts);
		}

		{
			Pupil::Renderer2D::ResetStats();
			m_Framebuffer->Bind();
		}

		{
			PP_PROFILE_SCOPE("Renderer::Prep");

			Pupil::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			Pupil::RenderCommand::Clear();
		}
#if 1
		{
			PP_PROFILE_SCOPE("Renderer::Draw");

			static float rotation = 0.0f;
			rotation += ts * 90.0f;

			Pupil::Renderer2D::BeginScene(m_OrthoCameraController.GetCamera());
			Pupil::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, { 20.0f, 20.0f }, m_Texture3, 20.0f);
			Pupil::Renderer2D::EndScene();

			Pupil::Renderer2D::BeginScene(m_OrthoCameraController.GetCamera());
			for (float y = -5.0f; y < 5.0f; y += 0.5f) {
				for (float x = -5.0f; x < 5.0f; x += 0.5f) {
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.6f, (y + 5.0f) / 10.0f, 0.7f };
					Pupil::Renderer2D::DrawQuad({ x, y, -0.15f }, { 0.45f, 0.45f }, color);
				}
			}
			Pupil::Renderer2D::DrawRotateQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, 45.0f, m_Color);
			Pupil::Renderer2D::DrawQuad({ -1.0f, 0.5f }, { 0.5f, 0.8f }, { 0.1f, 0.1f, 1.0f, 1.0f });
			Pupil::Renderer2D::DrawQuad({ 1.0f, -0.5f }, { 0.5f, 0.5f }, { 0.1f, 0.8f, 0.1f, 1.0f });
			Pupil::Renderer2D::DrawRotateQuad({ 1.0f, 0.5f }, { 0.5f, 0.5f }, rotation, m_Texture1);
			Pupil::Renderer2D::EndScene();

		}
#endif

		{
			PP_PROFILE_SCOPE("ParticleSystem");
			if (Pupil::Input::IsMousePressed(PP_MOUSE_BUTTON_LEFT)) {
				auto [x, y] = Input::GetMousePosition();
				auto width = Pupil::Application::Get().GetWindow().GetWidth();
				auto height = Pupil::Application::Get().GetWindow().GetHeight();

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

#if 0
		{
			PP_PROFILE_SCOPE("SpriteSheet");
			Pupil::Renderer2D::BeginScene(m_OrthoCameraController.GetCamera());
			for (int y = 0; y != m_MapHeight; ++y) {
				for (int x = 0; x != m_MapWidth; ++x) {
					char tileType = m_MapTiles[y * m_MapWidth + x];
					Pupil::Ref<Pupil::SubTexture2D> texture;
					if (m_SpriteMap.find(tileType) != m_SpriteMap.end()) {
						texture = m_SpriteMap[tileType];
						Pupil::Renderer2D::DrawQuad({ x, m_MapHeight - y }, { 1, 1 }, texture);
					}
					else Pupil::Renderer2D::DrawQuad({ x, m_MapHeight - y }, { 1, 1 }, m_Texture3);
				}
			}
			Pupil::Renderer2D::EndScene();
		}
#endif
		m_Framebuffer->UnBind();
	}

	void Sandbox2D::OnImGuiRender() {
		PP_PROFILE_FUNCTION();
		static bool dockingEnable = true;
		if (dockingEnable) {
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

					if (ImGui::MenuItem("Exit")) Pupil::Application::Get().Close();
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
			ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
			// show texture
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });

			ImGui::End();


			ImGui::End();
		}
		else {
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
			ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
			// show texture
			uint32_t textureID = m_Texture3->GetRendererID();
			ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f });

			ImGui::End();
		}
	}

	void Sandbox2D::OnEvent(Pupil::Event& event) {
		PP_PROFILE_FUNCTION();

		m_OrthoCameraController.OnEvent(event);
	}
	
	void Sandbox2D::OnDetach() {
		PP_PROFILE_FUNCTION();

	}

}