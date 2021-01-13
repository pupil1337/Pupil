#pragma once
#include "pupil/Core.h"

#include "pupil//Renderer/Buffer.h"

namespace Pupil {

	class PP_API VertexArray {
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& IndexBuffer) = 0;

		static VertexArray* Create();
	};


}