#pragma once
#include "pupil/Core/Core.h"

#include "pupil/Renderer/Shader.h"

#include <unordered_map>

namespace Pupil {

	class PP_API OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& filePath_WithOutDot);
		virtual ~OpenGLShader();

		void CheckCompileError(uint32_t shader, std::string type);
		inline virtual const std::string& GetName() const override { return m_Name; }
		virtual void Bind() const override;
		virtual void UnBind() const override;
	

	public:
		virtual void SetBool(const std::string& name, bool value) const override;
		virtual void SetInt(const std::string& name, int value) const override;
		virtual void SetFloat(const std::string& name, float value) const override;
		virtual void SetVec2(const std::string& name, float x, float y) const override;
		virtual void SetVec2(const std::string& name, const glm::vec2& vec2) const override;
		virtual void SetVec3(const std::string& name, float x, float y, float z) const override;
		virtual void SetVec3(const std::string& name, const glm::vec3& vec3) const override;
		virtual void SetVec4(const std::string& name, float x, float y, float z, float w) const override;
		virtual void SetVec4(const std::string& name, const glm::vec4& vec4) const override;
		virtual void SetMat2(const std::string& name, const glm::mat2& mat2) const override;
		virtual void SetMat3(const std::string& name, const glm::mat3& mat3) const override;
		virtual void SetMat4(const std::string& name, const glm::mat4& mat4) const override;
	private:
		int GetUniformLocation(const std::string& name) const;
		
	private:
		uint32_t m_RendererID;
		std::string m_Name;
		mutable std::unordered_map<std::string, int> m_UniformLocation;
	};
}