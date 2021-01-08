#include "pppch.h"
#include "Pupil.h"

#include "imgui/imgui.h"

class ExampleLayer :public Pupil::Layer {
public:
	ExampleLayer()
		:Layer("Example") {
	}

	virtual void OnUpdate() override {

	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	virtual void OnEvent(Pupil::Event& event) override {
		if (event.GetEventType() == Pupil::EventType::KeyPressed) {
			Pupil::KeyPressedEvent& e = (Pupil::KeyPressedEvent&)event;
			PP_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Pupil::Application {

public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}

};

Pupil::Application* Pupil::CreateApplication() {
	return new Sandbox();
}