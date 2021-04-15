#include "pppch.h"
#include "OpenGLFramebuffer.h"

namespace Pupil {

	static bool IsDepthFormt(FramebufferTextureFormt formt) {
		switch (formt) {
			case FramebufferTextureFormt::DEPTH32STENCIL8: return true;
		}
		return false;
	}

	void OpenGLFramebuffer::CreateColorAttachment(size_t index, GLint internalformat, GLenum formt, GLenum type, uint32_t width, uint32_t height, GLint level/*=0*/) {
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachments[index]);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[index]);

		glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, 0, formt, type, nullptr);
		
		glGenerateTextureMipmap(m_ColorAttachments[index]);// must after "glTexImage2D"
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, m_ColorAttachments[index], level);
	}

	void OpenGLFramebuffer::CreateDepthAttachment(GLint internalformat, GLenum format, GLenum type, GLenum attachment, uint32_t width, uint32_t height) {
		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);

		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_DepthAttachment, 0);
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec): m_Specification(spec) {
		
		for (auto texSpec : spec.Attachments.Attachments) {
			if (IsDepthFormt(texSpec.TextureFormt)) m_DepthTextureSpecification = texSpec;
			else m_ColorTextureSpecifications.push_back(texSpec);
		}

		Init();
	}

	void OpenGLFramebuffer::Init() {
		if (m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);
		}

		PP_CORE_ASSERT(m_ColorTextureSpecifications.size() <= 4, "OpenGL colorAttachments must <= 4");

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		
		if (m_ColorTextureSpecifications.size()) {
			m_ColorAttachments.resize(m_ColorTextureSpecifications.size());
			for (size_t i = 0; i != m_ColorTextureSpecifications.size(); ++i) {
				switch (m_ColorTextureSpecifications[i].TextureFormt) {
					case FramebufferTextureFormt::RGBA: CreateColorAttachment(i, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, m_Specification.Width, m_Specification.Height); break;
					case FramebufferTextureFormt::RED_INT: CreateColorAttachment(i, GL_R32I, GL_RED_INTEGER, GL_INT, m_Specification.Width, m_Specification.Height); break;
				}
			}
		}

		if (m_DepthTextureSpecification.TextureFormt != FramebufferTextureFormt::None) {
			switch (m_DepthTextureSpecification.TextureFormt) {
				case FramebufferTextureFormt::DEPTH32STENCIL8: CreateDepthAttachment(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height); break;
			}
		}

		PP_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer not completed");

		GLuint buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

		if (m_ColorAttachments.size() > 1) glDrawBuffers(m_ColorAttachments.size(), buffers);
		else if (m_ColorAttachments.empty()) {
			glDrawBuffer(GL_NONE);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::UnBind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
		m_Specification.Width = width > 0 ? width : 1;
		m_Specification.Height = height > 0 ? height : 1;
		Init();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentID, int x, int y) {
		PP_CORE_ASSERT(attachmentID < m_ColorAttachments.size(), "have not this attachmentID");

		Bind();// must!!!
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentID);
		int pixelData = -1;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		UnBind();
		return pixelData;
	}

}