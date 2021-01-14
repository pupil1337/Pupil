#include "pppch.h"
#include "RenderCommand.h"

#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace Pupil {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}