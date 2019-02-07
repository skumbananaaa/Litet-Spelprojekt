#include <EnginePch.h>
#include <Graphics/Materials/Material.h>

Material::Material()
	: m_Color(),
	m_pTexture(nullptr),
	m_pNormalMap(nullptr),
	m_CullMode(CULL_MODE_BACK)
{
	for (uint32 i = 0; i < NUM_CLIP_DISTANCES; i++)
	{
		m_ClipPlanesEnabled[i] = false;
	}
}

Material::~Material()
{
}