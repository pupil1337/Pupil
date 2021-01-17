#include "pppch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Pupil {

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) {

		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			PP_CORE_ERROR("{0}", infoLog.data());
			PP_CORE_ASSERT(false, "Vertex shader compiled failed!");

			// In this simple program, we'll just leave
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			PP_CORE_ERROR("{0}", infoLog.data());
			PP_CORE_ASSERT(false, "Fragment shader compiled failed!");

			// In this simple program, we'll just leave
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			PP_CORE_ERROR("{0}", infoLog.data());
			PP_CORE_ASSERT(false, "Shader link failed!");

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
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