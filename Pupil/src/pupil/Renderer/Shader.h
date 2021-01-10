#pragma once

#include <string>

#include "pupil/Core.h"

namespace Pupil {

	class PP_API Shader {

	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;
	private:
		uint32_t m_RendererID;
	};

}