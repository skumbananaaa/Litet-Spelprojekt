#pragma once
#include <EnginePch.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/GLContext.h>

class API Material
{
	friend class ResourceHandler;

public:
	Material();
	~Material();

	void SetCullMode(CULL_MODE mode);
	void SetClipPlane(const glm::vec3& normal, float distFromOrigin);
	void EnableClipPlane(bool enable);

	const Texture2D* GetNormalMap() const;
	const Texture2D* GetTexture() const;
	const glm::vec4& GetColor() const;
	const glm::vec4& GetClipPlane() const;
	CULL_MODE GetCullMode() const;

	bool HasTexture() const;
	bool HasNormalMap() const;
	bool ClipPlaneEnabled() const;

private:
	void SetTexture(const Texture2D* const pTexture);
	void SetNormalMap(const Texture2D* const pNormalMap);
	void SetColor(const glm::vec4& color);

private:
	const Texture2D* m_pTexture;
	const Texture2D* m_pNormalMap;
	glm::vec4 m_Color;
	glm::vec4 m_ClipPlane;
	CULL_MODE m_CullMode;
	bool m_ClipPlaneEnabled;
};

inline bool Material::HasTexture() const
{
	return m_pTexture != nullptr;
}

inline bool Material::HasNormalMap() const
{
	return m_pNormalMap != nullptr;
}

inline bool Material::ClipPlaneEnabled() const
{
	return m_ClipPlaneEnabled;
}

inline void Material::SetTexture(const Texture2D* const pTexture)
{
	m_pTexture = pTexture;
}

inline void Material::SetNormalMap(const Texture2D* const pNormalMap)
{
	m_pNormalMap = pNormalMap;
}

inline void Material::SetCullMode(CULL_MODE mode)
{
	m_CullMode = mode;
}

inline CULL_MODE Material::GetCullMode() const
{
	return m_CullMode;
}

inline void Material::SetClipPlane(const glm::vec3& normal, float distFromOrigin)
{
	m_ClipPlane.x = normal.x;
	m_ClipPlane.y = normal.y;
	m_ClipPlane.z = normal.z;
	m_ClipPlane.w = distFromOrigin;
}

inline void Material::EnableClipPlane(bool enable)
{
	m_ClipPlaneEnabled = enable;
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

inline const glm::vec4& Material::GetClipPlane() const
{
	return m_ClipPlane;
}
