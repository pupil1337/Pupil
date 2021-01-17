#pragma once

// for Application
#include "pupil/Application.h"

#include "pupil/Log.h"

#include "pupil/Input.h"
#include "pupil/KeyCodes.h"
#include "pupil/MouseButtonCodes.h"

#include "pupil/Layer.h"
#include "pupil/ImGui/ImGuiLayer.h"

// ------Core----------------------
#include "pupil/Core/TimeStep.h"
#include "pupil/Core/Material_BPhong.h"
// -------------------------------

// ------Renderer------------------
#include "pupil/Renderer/Shader.h"
#include "platform/OpenGL/OpenGLShader.h"
#include "pupil/Renderer/VertexArray.h"
#include "pupil/Renderer/Buffer.h"

#include "pupil/Renderer/Renderer.h"
#include "pupil/Renderer/RenderCommand.h"
// -------------------------------

#include "pupil/Renderer/OrthographicCamera.h"

// ------Entry Point--------------
#include "pupil/EntryPoint.h"
// -------------------------------