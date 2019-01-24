#include <EnginePch.h>
#include <Graphics/Materials/Material.h>

Material::Material()
	: m_Color(),
	m_pTexture(nullptr),
	m_pNormalMap(nullptr)
{
}

Material::~Material()
{
}