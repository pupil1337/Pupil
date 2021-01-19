#include "pppch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>

namespace Pupil {

	void OpenGLShader::checkCompileError(uint32_t shader, std::string type) {
		int success;
		char info_log[1024];
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, info_log);
				PP_CORE_ASSERT("ERROR::SHADER::COMPILE (type : - {0} -)\n{1}\n", info_log);
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, info_log);
				PP_CORE_ASSERT("ERROR::SHADER::LINK (type : - {0} -)\n{1}\n", info_log);
			}
		}
	}
	// -------------------OpenGLShader-------------------------------
	OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {

			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

			if (geometryPath != "") {
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure e) {
			PP_CORE_ASSERT(false, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ!");
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();


		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);
		checkCompileError(vertexShader, "VERTEX");

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);
		checkCompileError(fragmentShader, "FRAGMENT");

		GLuint geometry;
		if (geometryPath != "") {
			const char* gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileError(geometry, "GEOMETRY");
		}
		m_RendererID = glCreateProgram();
		GLuint shader_program = m_RendererID;
		glAttachShader(shader_program, vertexShader);
		glAttachShader(shader_program, fragmentShader);
		if (geometryPath != "") glAttachShader(shader_program, geometry);
		glLinkProgram(shader_program);
		checkCompileError(shader_program, "PROGRAM");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometryPath != "") glDeleteShader(geometry);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const {
		glUseProgram(0);
	}

	int OpenGLShader::GetUniformLocation(const std::string& name) const {
		if (m_UniformLocation.find(name) != m_UniformLocation.end()) {
			return m_UniformLocation[name];
		}
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		m_UniformLocation[name] = location;
		return location;
	}

	// Set Uniforms
	void OpenGLShader::SetBool(const std::string& name, bool value) const {
		glUniform1i(GetUniformLocation(name), (int)value);
	}

	void OpenGLShader::SetInt(const std::string& name, int value) const {
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value) const {
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetVec2(const std::string& name, float x, float y) const {
		glUniform2f(GetUniformLocation(name), x, y);
	}

	void OpenGLShader::SetVec2(const std::string& name, const glm::vec2& vec2) const {
		glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(vec2));
	}

	void OpenGLShader::SetVec3(const std::string& name, float x, float y, float z) const {
		glUniform3f(GetUniformLocation(name), x, y, z);
	}

	void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& vec3) const {
		glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vec3));
	}

	void OpenGLShader::SetVec4(const std::string& name, float x, float y, float z, float w) const {
		glUniform4f(GetUniformLocation(name), x, y, z, w);
	}

	void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& vec4) const {
		glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(vec4));
	}

	void OpenGLShader::SetMat2(const std::string& name, const glm::mat2& mat2) const {
		glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat2));
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& mat3) const {
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat3));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat4) const {
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat4));
	}

}