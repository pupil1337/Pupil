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
		m_Texture = Pupil::Texture2D::Create("assets/textures/awesomeface.png");
	}

	void Sandbox2D::OnUpdate(Pupil::TimeStep ts) {
		m_TimeStep = ts;
		PP_PROFILE_FUNCTION();
		{
			PP_PROFILE_SCOPE("OrthoCameraController::OnUpdate");
			m_OrthoCameraController.OnUpdate(ts);
		}
		{
			PP_PROFILE_SCOPE("Renderer::Prep");
			Pupil::RenderCommand::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
			Pupil::RenderCommand::Clear();
		}
		{
			PP_PROFILE_SCOPE("Renderer::Rraw");
			Pupil::Renderer2D::BeginScene(m_OrthoCameraController.GetCamera());
			Pupil::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_Color);
			Pupil::Renderer2D::DrawQuad({ -1.0f, 0.5f }, { 0.5f, 0.8f }, { 0.1f, 0.1f, 1.0f, 1.0f });
			Pupil::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_Texture);
			Pupil::Renderer::EndScene();
		}
	}

	void Sandbox2D::OnImGuiRender() {
		ImGui::Begin("Settings");
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
		m_OrthoCameraController.OnEvent(event);
	}
	
	void Sandbox2D::OnDetach() {

	}

}