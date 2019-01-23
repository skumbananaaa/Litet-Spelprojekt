#pragma once
#include <EnginePch.h>
#include <Graphics/Textures/Texture2D.h>

class API Material
{
public:
	Material();
	~Material();

	void SetTexture(const Texture2D* pTexture);
	void SetColor(const glm::vec4& color);
	const glm::vec4& GetColor() const;
	const Texture2D* GetTexture() const;
	bool HasTexture() const;

private:
	const Texture2D* m_pTexture;
	glm::vec4 m_Color;
};

inline bool Material::HasTexture() const
{
	return m_pTexture != nullptr;
}

inline void Material::SetTexture(const Texture2D* const pTexture)
{
	m_pTexture = pTexture;
}

inline const Texture2D* Material::GetTexture() const
{
	return m_pTexture;
}

inline void Material::SetColor(const glm::vec4& color)
{
	m_Color = color;
}

inline const glm::vec4& Material::GetColor() const
{
	return m_Color;
}
