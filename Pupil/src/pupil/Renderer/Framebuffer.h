#pragma once
#include "pupil/Core/Core.h"

namespace Pupil {

	enum class FramebufferTextureFormt {
		None = 0,
		RGBA,
		RED_INT,
		DEPTH32STENCIL8
	};

	struct FramebufferTextureSpecification {
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormt formt): TextureFormt(formt) { }

		/// data ///
		FramebufferTextureFormt TextureFormt = FramebufferTextureFormt::None;
		// ToDo: wrap
	};

	struct FramebufferAttachmentSpecification {
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments): Attachments(attachments) { }

		/// data ///
		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification {
		
		/// data ///
		uint32_t Width, Height;
		FramebufferAttachmentSpecification Attachments;
	};

	class Framebuffer {
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentID, int x, int y) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual uint32_t GetDepthAttachmentRendererID() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

	};

}