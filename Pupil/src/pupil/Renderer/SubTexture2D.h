#pragma once
#include "pupil/Core/Core.h"

#include "pupil/Renderer/Texture.h"
#include "glm/glm.hpp"

namespace Pupil {

	class SubTexture2D {
	public:
		SubTexture2D(const Ref<Texture2D>& texture2D, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D> GetTexture2D() const { return m_Texture2D; }
		const glm::vec2* GetTextureCoords() const { return m_TextureCoords; }

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture2D, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spritSize = { 1.0f, 1.0f });

	private:
		Ref<Texture2D> m_Texture2D;
		glm::vec2 m_TextureCoords[4];
	};

}