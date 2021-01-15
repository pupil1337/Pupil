#pragma once

#include <string>

#include "pupil/Core.h"

#include <glm/glm.hpp>

namespace Pupil {

	class PP_API Shader {

	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

		void SetMat4(const std::string& name, const glm::mat4& mat4);
	private:
		uint32_t m_RendererID;
	};

}