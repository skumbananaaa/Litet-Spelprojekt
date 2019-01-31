#include <Graphics/SkyBox.h>

SkyBox::SkyBox(TextureCube * texture): m_Mesh(IndexedMesh::CreateCube())
{
	m_TextureCube = texture;
}

SkyBox::~SkyBox()
{
	delete m_Mesh;
	delete m_TextureCube;
}

TextureCube * SkyBox::GetTexture()
{
	return m_TextureCube;
}

IndexedMesh * SkyBox::GetMesh()
{
	return m_Mesh;
}
