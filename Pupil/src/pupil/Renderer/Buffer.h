#pragma once

#include "pupil/Core.h"

namespace Pupil {

	class PP_API VertexBuffer {
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class PP_API IndexBuffer {
	public :
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static IndexBuffer* Create(uint32_t* vertices, uint32_t count);
	};

}