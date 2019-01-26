#pragma once
#include <EnginePch.h>
#include <Graphics/Textures/Texture2D.h>

class API Material
{
public:
	Material();
	~Material();

	void SetTexture(const Texture2D* const pTexture);
	void SetNormalMap(const Texture2D* const pNormalMap);
	void SetColor(const glm::vec4& color);
	const glm::vec4& GetColor() const;
	const Texture2D* GetTexture() const;
	const Texture2D* GetNormalMap() const;
	bool HasTexture() const;
	bool HasNormalMap() const;

private:
	const Texture2D* m_pTexture;
	const Texture2D* m_pNormalMap;
	glm::vec4 m_Color;
	glm::mat4 d;
};

inline bool Material::HasTexture() const
{
	return m_pTexture != nullptr;
}

inline bool Material::HasNormalMap() const
{
	return m_pNormalMap != nullptr;
}

inline void Material::SetTexture(const Texture2D* const pTexture)
{
	m_pTexture = pTexture;
}

inline void Material::SetNormalMap(const Texture2D* const pNormalMap)
{
	m_pNormalMap = pNormalMap;
}

inline const Texture2D* Material::GetTexture() const
{
	return m_pTexture;
}

inline const Texture2D* Material::GetNormalMap() const
{
	return m_pNormalMap;
}

inline void Material::SetColor(const glm::vec4& color)
{
	m_Color = color;
}

inline const glm::vec4& Material::GetColor() const
{
	return m_Color;
}
