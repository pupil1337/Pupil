#pragma once

#include "pupil/Core/Core.h"

namespace Pupil {

	enum class ShaderDataType {
		None = 0,
		Bool,
		Int, Int2, Int3, Int4,
		Float, Float2, Float3, Float4,
		Mat3, Mat4
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case Pupil::ShaderDataType::Bool  : return 1;
			case Pupil::ShaderDataType::Int   : return 4 * 1;
			case Pupil::ShaderDataType::Int2  : return 4 * 2;
			case Pupil::ShaderDataType::Int3  : return 4 * 3;
			case Pupil::ShaderDataType::Int4  : return 4 * 4;
			case Pupil::ShaderDataType::Float : return 4 * 1;
			case Pupil::ShaderDataType::Float2: return 4 * 2;
			case Pupil::ShaderDataType::Float3: return 4 * 3;
			case Pupil::ShaderDataType::Float4: return 4 * 4;
			case Pupil::ShaderDataType::Mat3  : return 4 * (3 * 3);
			case Pupil::ShaderDataType::Mat4  : return 4 * (4 * 4);
		}

		PP_CORE_ASSERT(false, "Unknow ShaderDataTypeSize!");
		return 0;
	}

	struct BufferElement {
		// input
		ShaderDataType Type;
		std::string Name;
		bool Normalized;
		// to calculate
		uint32_t Size;
		uint32_t Offset;

		BufferElement() { }
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Type(type), Name(name), Size(ShaderDataTypeSize(type)), Normalized(normalized), Offset(0) { }
		
		uint32_t GetComponetCount(ShaderDataType type) const {
			switch (type) {
				case Pupil::ShaderDataType::Bool  : return 1;
				case Pupil::ShaderDataType::Int   : return 1;
				case Pupil::ShaderDataType::Int2  : return 2;
				case Pupil::ShaderDataType::Int3  : return 3;
				case Pupil::ShaderDataType::Int4  : return 4;
				case Pupil::ShaderDataType::Float : return 1;
				case Pupil::ShaderDataType::Float2: return 2;
				case Pupil::ShaderDataType::Float3: return 3;
				case Pupil::ShaderDataType::Float4: return 4;
				case Pupil::ShaderDataType::Mat3  : return 3 * 3;
				case Pupil::ShaderDataType::Mat4  : return 4 * 4;
			}

			PP_CORE_ASSERT(false, "Return GetComponetCount failed!");
			return 0;
		}

	};

	///  BufferLayout ///
	class PP_API BufferLayout {
	public:
		BufferLayout() { }

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements) {
			CalculateOffsetsAndStride();
		}

		inline const std::vector<BufferElement>& GetElement() const { return m_Elements; }
		inline const uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return  m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return  m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return  m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return  m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride() {
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};

	/// VertexBuffer ///
	class PP_API VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void SetData(const void* data, uint32_t size) const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};

	/// IndexBuffer ///
	class PP_API IndexBuffer {
	public :
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		
		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* vertices, uint32_t count);
	};

}