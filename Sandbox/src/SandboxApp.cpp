#include "pppch.h"
#include "Pupil.h"
#include "pupil/Core/EntryPoint.h"

#include "ExampleLayer.h"
#include "Sandbox2D.h"

class Sandbox : public Pupil::Application {

public:
	Sandbox() {
		//PushLayer(new Pupil::ExampleLayer());
		PushLayer(new Pupil::Sandbox2D());
	}

	~Sandbox() {

	}

};

Pupil::Application* Pupil::CreateApplication() {
	return new Sandbox();
}