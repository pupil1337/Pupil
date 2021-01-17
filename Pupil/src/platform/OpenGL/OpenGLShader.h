#pragma once
#include "pupil/Core.h"

#include "pupil/Renderer/Shader.h"

#include <unordered_map>

namespace Pupil {

	class PP_API OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();
		virtual void Bind() const override;
		virtual void UnBind() const override;
	

	public:
		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetVec2(const std::string& name, float x, float y) const;
		void SetVec2(const std::string& name, const glm::vec2& vec2) const;
		void SetVec3(const std::string& name, float x, float y, float z) const;
		void SetVec3(const std::string& name, const glm::vec3& vec3) const;
		void SetVec4(const std::string& name, float x, float y, float z, float w) const;
		void SetVec4(const std::string& name, const glm::vec4& vec4) const;
		void SetMat2(const std::string& name, const glm::mat2& mat2) const;
		void SetMat3(const std::string& name, const glm::mat3& mat3) const;
		void SetMat4(const std::string& name, const glm::mat4& mat4) const;
	private:
		int GetUniformLocation(const std::string& name) const;
		
	private:
		uint32_t m_RendererID;
		mutable std::unordered_map<std::string, int> m_UniformLocation;
	};
}