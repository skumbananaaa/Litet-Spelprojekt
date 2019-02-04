#include <EnginePch.h>
#include <Graphics/SkyBox.h>

SkyBox::SkyBox(TextureCube* texture)
{
	m_pMesh = ResourceHandler::GetMesh(MESH::CUBE);
	m_pTextureCube = texture;
}

SkyBox::~SkyBox()
{
}

const TextureCube& SkyBox::GetTexture() const noexcept
{
	return *m_pTextureCube;
}

const IndexedMesh& SkyBox::GetMesh() const noexcept
{
	return *m_pMesh;
}