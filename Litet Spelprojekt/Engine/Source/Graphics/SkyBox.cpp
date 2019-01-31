#include <EnginePch.h>
#include <Graphics/SkyBox.h>

SkyBox::SkyBox(TextureCube * texture): m_pMesh(IndexedMesh::CreateCube())
{
	m_pTextureCube = texture;
}

SkyBox::~SkyBox()
{
	delete m_pMesh;
	delete m_pTextureCube;
}

TextureCube & SkyBox::GetTexture() const noexcept
{
	return *m_pTextureCube;
}

IndexedMesh& SkyBox::GetMesh() const noexcept
{
	return *m_pMesh;
}