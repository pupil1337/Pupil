#include "pppch.h"
#include "SubTexture2D.h"

namespace Pupil {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture2D, const glm::vec2& min, const glm::vec2& max)
		: m_Texture2D(texture2D) {
		m_TextureCoords[0] = { min.x, min.y };
		m_TextureCoords[1] = { max.x, min.y };
		m_TextureCoords[2] = { max.x, max.y };
		m_TextureCoords[3] = { min.x, max.y };
	}

	Pupil::Ref<Pupil::SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture2D, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spritSize) {
		glm::vec2 min = { coords.x * cellSize.x / texture2D->GetWidth(), coords.y * cellSize.y / texture2D->GetHeight() };
		glm::vec2 max = { (coords.x+spritSize.x) * cellSize.x / texture2D->GetWidth(), (coords.y+spritSize.y) * cellSize.y / texture2D->GetHeight() };
		return std::make_shared<SubTexture2D>(texture2D, min, max);
	}

}