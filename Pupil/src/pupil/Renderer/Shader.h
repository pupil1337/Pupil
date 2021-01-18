#pragma once
#include "pupil/Core.h"

#include <string>
#include <glm/glm.hpp>

namespace Pupil {

	class PP_API Shader {

	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};

}