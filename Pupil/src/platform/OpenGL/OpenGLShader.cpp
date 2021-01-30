#include "pppch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>

namespace Pupil {

	// -------------------OpenGLShader-------------------------------
	OpenGLShader::OpenGLShader(const std::string& filePath_WithOutDot) {
		PP_PROFILE_FUNCTION();

		auto pos = filePath_WithOutDot.find_last_of("/\\");
		if (pos == std::string::npos) m_Name = filePath_WithOutDot;
		else m_Name = filePath_WithOutDot.substr(pos + 1);

		std::string vertexPath = filePath_WithOutDot + ".vert";
		std::string fragmentPath = filePath_WithOutDot + ".frag";
		std::string geometryPath = filePath_WithOutDot + ".geom";
		std::ifstream f(geometryPath.c_str());
		if (!f.good()) geometryPath = "";
	
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
		CheckCompileError(vertexShader, "VERTEX");

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);
		CheckCompileError(fragmentShader, "FRAGMENT");

		GLuint geometry;
		if (geometryPath != "") {
			const char* gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			CheckCompileError(geometry, "GEOMETRY");
		}
		m_RendererID = glCreateProgram();
		GLuint shader_program = m_RendererID;
		glAttachShader(shader_program, vertexShader);
		glAttachShader(shader_program, fragmentShader);
		if (geometryPath != "") glAttachShader(shader_program, geometry);
		glLinkProgram(shader_program);
		CheckCompileError(shader_program, "PROGRAM");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometryPath != "") glDeleteShader(geometry);
	}

	void OpenGLShader::CheckCompileError(uint32_t shader, std::string type) {
		PP_PROFILE_FUNCTION();

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

	OpenGLShader::~OpenGLShader() {
		PP_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const {
		PP_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const {
		PP_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	int OpenGLShader::GetUniformLocation(const std::string& name) const {
		PP_PROFILE_FUNCTION();

		if (m_UniformLocation.find(name) != m_UniformLocation.end()) {
			return m_UniformLocation[name];
		}
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		m_UniformLocation[name] = location;
		return location;
	}

	// Set Uniforms
	void OpenGLShader::SetBool(const std::string& name, bool value) const {
		PP_PROFILE_FUNCTION();

		glUniform1i(GetUniformLocation(name), (int)value);
	}

	void OpenGLShader::SetInt(const std::string& name, int value) const {
		PP_PROFILE_FUNCTION();

		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* value, uint32_t count) const {
		PP_PROFILE_FUNCTION();

		glUniform1iv(GetUniformLocation(name), count, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value) const {
		PP_PROFILE_FUNCTION();

		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::SetVec2(const std::string& name, float x, float y) const {
		PP_PROFILE_FUNCTION();

		glUniform2f(GetUniformLocation(name), x, y);
	}

	void OpenGLShader::SetVec2(const std::string& name, const glm::vec2& vec2) const {
		PP_PROFILE_FUNCTION();

		glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(vec2));
	}

	void OpenGLShader::SetVec3(const std::string& name, float x, float y, float z) const {
		PP_PROFILE_FUNCTION();

		glUniform3f(GetUniformLocation(name), x, y, z);
	}

	void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& vec3) const {
		PP_PROFILE_FUNCTION();

		glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vec3));
	}

	void OpenGLShader::SetVec4(const std::string& name, float x, float y, float z, float w) const {
		PP_PROFILE_FUNCTION();

		glUniform4f(GetUniformLocation(name), x, y, z, w);
	}

	void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& vec4) const {
		PP_PROFILE_FUNCTION();

		glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(vec4));
	}

	void OpenGLShader::SetMat2(const std::string& name, const glm::mat2& mat2) const {
		PP_PROFILE_FUNCTION();

		glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat2));
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& mat3) const {
		PP_PROFILE_FUNCTION();

		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat3));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat4) const {
		PP_PROFILE_FUNCTION();

		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat4));
	}

}