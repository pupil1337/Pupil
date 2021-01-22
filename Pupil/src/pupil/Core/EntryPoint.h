#pragma once

#ifdef PP_PLATFORM_WINDOWS

extern Pupil::Application* Pupil::CreateApplication();

int main(int argc, char** argv) {

	Pupil::Log::Init();
	PP_CORE_INFO("Initialized Log!");

	Pupil::Application* app = Pupil::CreateApplication();
	app->Run();
	delete app;

	return 0;
}

#endif