#pragma once
#include "pupil/Core/Core.h"

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

		virtual void SetBool(const std::string& name, bool value) const = 0;
		virtual void SetInt(const std::string& name, int value) const = 0;
		virtual void SetIntArray(const std::string& name, int* value, uint32_t count) const = 0;
		virtual void SetFloat(const std::string& name, float value) const = 0;
		virtual void SetVec2(const std::string& name, float x, float y) const = 0;
		virtual void SetVec2(const std::string& name, const glm::vec2& vec2) const = 0;
		virtual void SetVec3(const std::string& name, float x, float y, float z) const = 0;
		virtual void SetVec3(const std::string& name, const glm::vec3& vec3) const = 0;
		virtual void SetVec4(const std::string& name, float x, float y, float z, float w) const = 0;
		virtual void SetVec4(const std::string& name, const glm::vec4& vec4) const = 0;
		virtual void SetMat2(const std::string& name, const glm::mat2& mat2) const = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& mat3) const = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& mat4) const = 0;

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