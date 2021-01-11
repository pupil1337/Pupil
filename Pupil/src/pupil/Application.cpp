#include "pppch.h"
#include "Application.h"

#include "glad/glad.h"

#include "pupil/Input.h"

namespace Pupil {

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type){
		switch (type) {
			case Pupil::ShaderDataType::Bool:   return GL_BOOL;
			case Pupil::ShaderDataType::Int:    return GL_INT;
			case Pupil::ShaderDataType::Int2:   return GL_INT;
			case Pupil::ShaderDataType::Int3:   return GL_INT;
			case Pupil::ShaderDataType::Int4:   return GL_INT;
			case Pupil::ShaderDataType::Float:  return GL_FLOAT;
			case Pupil::ShaderDataType::Float2: return GL_FLOAT;
			case Pupil::ShaderDataType::Float3: return GL_FLOAT;
			case Pupil::ShaderDataType::Float4: return GL_FLOAT;
			case Pupil::ShaderDataType::Mat3:   return GL_FLOAT;
			case Pupil::ShaderDataType::Mat4:   return GL_FLOAT;
		}

		PP_CORE_ASSERT(false, "Cant make this ShaderDataType to OpenGLBaseType!");
		return 0;
	}

	Application::Application() {
		
		PP_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(PP_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		// -------------------------------------

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,  0.2f,  0.3f, 0.3f, 1.0f,
			 0.0f,  0.5f, 0.0f,  0.8f,  0.8f, 0.2f, 1.0f,
			 0.5f, -0.5f, 0.0f,  0.8f,  0.2f, 0.8f, 1.0f,
		};
		uint32_t indices[3] = { 0, 1, 2 };

		glGenVertexArrays(1, &m_VertexArrays);
		glBindVertexArray(m_VertexArrays);

		m_VertexBuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
		m_VertexBuffer->Bind();
		
		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "aPos"},
				{ ShaderDataType::Float4, "aColor"}
			};
			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponetCount(element.Type),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			++index;
		}

		m_IndexBuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_IndexBuffer->Bind();

		// --shader-----------------------------------------
		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 aPos;
			layout(location = 1) in vec4 aColor;

			out vec3 FragPos;			
			out vec4 Color;
			
			void main() {
				FragPos = aPos;
				Color = aColor;
				gl_Position = vec4(aPos, 1.0f);
			}
		)";
		
		std::string fragmentSrc = R"(
			#version 330 core
			out vec4 FragColor;

			in vec3 FragPos;
			in vec4 Color;
			
			void main() {
				FragColor = vec4(Color);
			}
		)";


		m_Shader = std::unique_ptr<Shader>(new Pupil::Shader(vertexSrc, fragmentSrc));
	}
	
	Application::~Application() {
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlayer(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(PP_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
			(*it)->OnEvent(e);
			if (e.m_Handled) break;
			
		}

	}

	void Application::Run() {

		m_Shader->Bind();
		while (m_Running) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowClosedEvent& e) {
		m_Running = false;
		return true;
	}
}