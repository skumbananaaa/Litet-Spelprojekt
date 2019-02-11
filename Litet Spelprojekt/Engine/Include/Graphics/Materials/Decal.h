#pragma once
#include <EnginePch.h>
#include <Graphics/Textures/Texture2D.h>

class API Decal
{
	friend class ResourceHandler;

public:
	const Texture2D* GetTexture() const;
	const Texture2D* GetNormalMap() const;
	bool HasTexture() const;
	bool HasNormalMap() const;

private:
	Decal();
	~Decal();

	void SetDiffuseMap(const Texture2D* const pTexture);
	void SetNormalMap(const Texture2D* const pNormalMap);

private:
	const Texture2D* m_pTexture;
	const Texture2D* m_pNormalMap;
};

inline bool Decal::HasTexture() const
{
	return m_pTexture != nullptr;
}

inline bool Decal::HasNormalMap() const
{
	return m_pNormalMap != nullptr;
}

inline void Decal::SetDiffuseMap(const Texture2D* const pTexture)
{
	m_pTexture = pTexture;
}

inline void Decal::SetNormalMap(const Texture2D* const pNormalMap)
{
	m_pNormalMap = pNormalMap;
}

inline const Texture2D* Decal::GetTexture() const
{
	return m_pTexture;
}

inline const Texture2D* Decal::GetNormalMap() const
{
	return m_pNormalMap;
}