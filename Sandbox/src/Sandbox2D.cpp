#include "pppch.h"
#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		m_StairsTexture = Pupil::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128, 128 }, { 1, 1 });
		m_BushesTexture = Pupil::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 3 }, { 128, 128 }, { 1, 1 });
		m_TreeTexture   = Pupil::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 1 }, { 128, 128 }, { 1, 2 });

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
		}

		{
			PP_PROFILE_SCOPE("Renderer::Prep");

			Pupil::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			Pupil::RenderCommand::Clear();
		}
#if 0
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

		{
			PP_PROFILE_SCOPE("SpriteSheet");
			Pupil::Renderer2D::BeginScene(m_OrthoCameraController.GetCamera());
			Pupil::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_BushesTexture);
			Pupil::Renderer2D::DrawQuad({ 1.0f, 0.0f }, { 1.0f, 1.0f }, m_StairsTexture);
			Pupil::Renderer2D::DrawQuad({ 2.0f, 0.0f }, { 1.0f, 2.0f }, m_TreeTexture);
			Pupil::Renderer2D::EndScene();
		}
	}

	void Sandbox2D::OnImGuiRender() {
		PP_PROFILE_FUNCTION();

		ImGui::Begin("Settings");

		auto stats = Pupil::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads:      %d", stats.QuadCounts);
		ImGui::Text("Vertices:   %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices:    %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));

		ImGui::End();

		// performance PlotLines
		ImGui::Begin("Renderer Performance");
		m_FrameTimeGraph[values_offset] = m_TimeStep.GetMilliSecond();
		values_offset = (values_offset + 1) % 100;
		ImGui::PlotLines("#FrameTime", m_FrameTimeGraph, 100, values_offset, "FrameTime (ms)", 0.0f, 200.0f, ImVec2(0, 100), 100);
		ImGui::Text("FrameTime: %.2f (Fps: %d)", m_TimeStep.GetMilliSecond(), (int)(1.0f / m_TimeStep.GetSecond()));
		ImGui::End();
	}

	void Sandbox2D::OnEvent(Pupil::Event& event) {
		PP_PROFILE_FUNCTION();

		m_OrthoCameraController.OnEvent(event);
	}
	
	void Sandbox2D::OnDetach() {
		PP_PROFILE_FUNCTION();

	}

}