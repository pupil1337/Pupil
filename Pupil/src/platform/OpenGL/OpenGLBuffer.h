#pragma once

#include "pupil/Core/Core.h"

#include "pupil/Renderer/Buffer.h"

namespace Pupil {

	/// VertexBuffer ///
	class PP_API OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer() override;

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetData(const void* data, uint32_t size) const override;

		inline virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		inline virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	/// IndexBuffer ///
	class PP_API OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

		virtual void Bind() const;
		virtual void UnBind() const;

		inline uint32_t GetCount() const { return m_count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_count;
	};

}