#pragma once
#include "pupil/Core/Core.h"

#include "pupil/Renderer/Framebuffer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Pupil {

	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer() override;

		void Init();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		inline virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { return m_ColorAttachments[index]; };
		inline virtual uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment; };
	
	private:
		void CreateColorAttachment(size_t index, GLint internalformat, GLenum formt, GLenum type, uint32_t width, uint32_t height, GLint level = 0);
		void CreateDepthAttachment(GLint internalformat, GLenum format, GLenum type, GLenum attachment, uint32_t width, uint32_t height);

	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorTextureSpecifications;
		FramebufferTextureSpecification m_DepthTextureSpecification { FramebufferTextureFormt::None };

		// attachment ID
		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};

}