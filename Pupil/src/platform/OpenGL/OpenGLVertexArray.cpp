#include "pppch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Pupil {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
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
	///////////////////////////////////////////////////////////////////// 
	///// VertexArray ///////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &m_RendererID);
	}

	void Pupil::OpenGLVertexArray::Bind() const {
		glBindVertexArray(m_RendererID);
	}

	void Pupil::OpenGLVertexArray::UnBind() const {
		glBindVertexArray(0);
	}

	void Pupil::OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
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

		vertexBuffer->UnBind();
		glBindVertexArray(0);

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void Pupil::OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& IndexBuffer) {
		glBindVertexArray(m_RendererID);
		IndexBuffer->Bind();

		IndexBuffer->UnBind();
		glBindVertexArray(0);

		m_IndexBuffer = IndexBuffer;
	}
}