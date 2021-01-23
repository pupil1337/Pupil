#pragma once

#ifdef PP_PLATFORM_WINDOWS

extern Pupil::Application* Pupil::CreateApplication();

int main(int argc, char** argv) {

	Pupil::Log::Init();
	
	PP_PROFILE_BEGIN_SESSION("APPCreate", "Pupil-Create.json");
	Pupil::Application* app = Pupil::CreateApplication();
	PP_PROFILE_END_SESSION();

	PP_PROFILE_BEGIN_SESSION("APPRun", "Pupil-Run.json");
	app->Run();
	PP_PROFILE_END_SESSION();

	PP_PROFILE_BEGIN_SESSION("APPDelete", "Pupil-Delete.json");
	delete app;
	PP_PROFILE_END_SESSION();

	return 0;
}

#endif