#pragma once
#include "pupil/Core.h"

#include "pupil//Renderer/Buffer.h"

namespace Pupil {

	class PP_API VertexArray {
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& IndexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer> >& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};


}