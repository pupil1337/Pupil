#include "pppch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

namespace Pupil {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path) {

		// ---load Data---
		bool irradianceCorrection = false;
		int width, height, nrComponents;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
		PP_CORE_ASSERT(data, "Failed to load texture2D!");
		m_Width = width;
		m_Height = height;
		if (nrComponents == 1) {
			InternalFormat = GL_RED;
			DataFormat = GL_RED;
		}
		else if (nrComponents == 3) {
			InternalFormat = irradianceCorrection ? GL_SRGB : GL_RGB;
			DataFormat = GL_RGB;
		}
		else if (nrComponents == 4) {
			InternalFormat = irradianceCorrection ? GL_SRGB_ALPHA : GL_RGBA8;
			DataFormat = GL_RGBA;
		}
		PP_CORE_ASSERT(InternalFormat & DataFormat, "have not this format!");
		
		// ---Create Texture---
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, InternalFormat, m_Width, m_Height);
		glGenerateTextureMipmap(m_RendererID);
		
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, DataFormat, GL_UNSIGNED_BYTE, data);
		
		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
	: m_Width(width), m_Height(height) {

		InternalFormat = GL_RGBA8;
		DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) const {
		uint32_t formatSize = DataFormat == GL_RGBA ? 4 : 3;
		PP_CORE_ASSERT(size == formatSize * m_Width * m_Height, "data must be entire the texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, DataFormat, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererID);
	}

}