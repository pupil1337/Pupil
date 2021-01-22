#include "pppch.h"
#include "RenderCommand.h"

#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace Pupil {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = std::make_unique<OpenGLRendererAPI>();
}