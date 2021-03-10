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

			Pupil::RenderCommand::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
			Pupil::RenderCommand::Clear();
		}
		{
			PP_PROFILE_SCOPE("Renderer::Draw");

			static float rotation = 0.0f;
			rotation += ts * 90.0f;

			Pupil::Renderer2D::BeginScene(m_OrthoCameraController.GetCamera());
			Pupil::Renderer2D::DrawRotateQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, 45.0f, m_Color);
			Pupil::Renderer2D::DrawQuad({ -1.0f, 0.5f }, { 0.5f, 0.8f }, { 0.1f, 0.1f, 1.0f, 1.0f });
			Pupil::Renderer2D::DrawQuad({  1.0f, -0.5f }, { 0.5f, 0.5f }, { 0.1f, 0.8f, 0.1f, 1.0f });
			Pupil::Renderer2D::DrawQuad({ 4.6f, 4.6f, -0.1f }, { 10.0f, 10.0f }, m_Texture1, 6.0f);
			Pupil::Renderer2D::DrawRotateQuad({ -4.6f, -4.6f, -0.1f }, { 10.0f, 10.0f }, 45.0f, m_Texture2, 6.0f);
			Pupil::Renderer2D::DrawRotateQuad({  1.0f, 0.5f }, { 0.5f, 0.5f }, rotation, m_Texture1);
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