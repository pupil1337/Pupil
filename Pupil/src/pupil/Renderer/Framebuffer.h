#pragma once
#include "pupil/Core/Core.h"

namespace Pupil {

	struct FramebufferSpecification {
		uint32_t Width, Height;
	};

	class Framebuffer {
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual uint32_t GetDepthAttachmentRendererID() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

	};

}