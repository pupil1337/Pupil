#include"pppch.h"
#include"Pupil.h"

class ExampleLayer :public Pupil::Layer {
public:
	ExampleLayer()
		:Layer("Example") {
	}

	void OnUpdate() override {
		PP_INFO("ExampleLayer::Update");
	}

	void OnEvent(Pupil::Event& event) override {
		PP_TRACE("{0}", event);
	}
};

class Sandbox : public Pupil::Application {

public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Pupil::ImGuiLayer());
	}

	~Sandbox() {

	}

};

Pupil::Application* Pupil::CreateApplication() {
	return new Sandbox();
}