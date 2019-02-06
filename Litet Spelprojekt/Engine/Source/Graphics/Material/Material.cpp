#include <EnginePch.h>
#include <Graphics/Materials/Material.h>

Material::Material()
	: m_Color(),
	m_ClipPlane(),
	m_pTexture(nullptr),
	m_pNormalMap(nullptr),
	m_CullMode(CULL_MODE_BACK),
	m_ClipPlaneEnabled(false)
{
}

Material::~Material()
{
}