#include "pppch.h"
#include "Pupil.h"

class ExampleLayer :public Pupil::Layer {
public:
	ExampleLayer()
		:Layer("Example") {
	}

	virtual void OnUpdate() override {
		
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