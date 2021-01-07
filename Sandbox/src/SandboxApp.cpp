#include "pppch.h"
#include "Pupil.h"

class ExampleLayer :public Pupil::Layer {
public:
	ExampleLayer()
		:Layer("Example") {
	}

<<<<<<< HEAD
	virtual void OnUpdate() override {

		if (Pupil::Input::IsKeyPressed(PP_KEY_TAB)) {
			PP_TRACE("Tab key is pressed");
		}
=======
	void OnUpdate() override {
		//PP_INFO("ExampleLayer::Update");
>>>>>>> fbb57b5050c9e8a49872d13e72b5d6ea531c356a
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