#pragma once
#include "pupil/Core/Core.h"

#include "pupil/Renderer/Framebuffer.h"

namespace Pupil {

	class OpenGLFramebuffer : public Framebuffer {
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer() override;

		void Init();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		inline virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; };
		inline virtual uint32_t GetDepthAttachmentRendererID() const override { return m_DepthAttachment; };
	
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;
	};

}