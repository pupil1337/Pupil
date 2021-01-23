#pragma once

// for Application
#include "pupil/Core/Application.h"

#include "pupil/Core/Log.h"

#include "pupil/Core/Input.h"
#include "pupil/Core/KeyCodes.h"
#include "pupil/Core/MouseButtonCodes.h"

#include "pupil/Core/Layer.h"
#include "pupil/ImGui/ImGuiLayer.h"

// ------Core----------------------
#include "pupil/Core/TimeStep.h"
#include "pupil/Core/Material_BPhong.h"
// -------------------------------

// ------Renderer------------------
#include "pupil/Renderer/Renderer.h"
#include "pupil/Renderer/Renderer2D.h"
#include "pupil/Renderer/Shader.h"
#include "pupil/Renderer/VertexArray.h"
#include "pupil/Renderer/Buffer.h"
#include "pupil/Renderer/Texture.h"

#include "platform/OpenGL/OpenGLShader.h"
// -------------------------------

// ------Debug--------------------
#include "pupil/Debuger/Instrumentoror.h"
// -------------------------------

// ------Camera-------------------
#include "pupil/Renderer/OrthographicCamera.h"
#include "pupil/Renderer/OrthographicCameraController.h"
// -------------------------------