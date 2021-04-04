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
#include "pupil/Renderer/Framebuffer.h"
#include "pupil/Renderer/VertexArray.h"
#include "pupil/Renderer/Buffer.h"
#include "pupil/Renderer/Texture.h"
#include "pupil/Renderer/SubTexture2D.h"

#include "platform/OpenGL/OpenGLShader.h"
// -------------------------------

// ------Camera-------------------
#include "pupil/Renderer/OrthographicCamera.h"
#include "pupil/Renderer/OrthographicCameraController.h"
// -------------------------------

// ------ECS----------------------
#include "pupil/Scene/Scene.h"
#include "pupil/Scene/Component.h"
#include "pupil/Scene/Entity.h"

// ------About UI-----------------
#include "panel/ScenePanel.h"