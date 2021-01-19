#pragma once
#include "pupil/Core.h"

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Pupil {

	class PP_API Shader {

	public:
		virtual ~Shader() = default;

		virtual const std::string& GetName() const = 0;
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static Ref<Shader> Create(const std::string& filePath_WithOutDot);
	};

	class PP_API ShaderLibrary {
	private:
		void Add(const std::string& name, const Ref<Shader>& shader);
		bool Exist(const std::string& name);

	public:
		void Load(const std::string& name, const std::string& filePath_WithOutDot);
		void Load(const std::string& filePath_WithOutDot);
		Ref<Shader> GetShader(const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}