#include "pppch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Pupil {


	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path) {
		bool irradianceCorrection = false;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

		int width, height, nrComponents;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
		m_Width = width;
		m_Height = height;
		PP_CORE_ASSERT(data, "Failed to load texture2D!");
		{
			GLenum internalFormat = 0;
			GLenum format = 0;
			if (nrComponents == 1) {
				internalFormat = GL_RED;
				format = GL_RED;
			}
			else if (nrComponents == 3) {
				internalFormat = irradianceCorrection ? GL_SRGB : GL_RGB;
				format = GL_RGB;
			}
			else if (nrComponents == 4) {
				internalFormat = irradianceCorrection ? GL_SRGB_ALPHA : GL_RGBA;
				format = GL_RGBA;
			}
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererID);
	}

}