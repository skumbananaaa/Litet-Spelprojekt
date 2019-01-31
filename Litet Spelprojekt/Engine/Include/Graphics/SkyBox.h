#pragma once
#include "Textures/TextureCube.h"
#include "Geometry/IndexedMesh.h"
class API SkyBox
{
private:
	IndexedMesh * m_Mesh;
	TextureCube * m_TextureCube;
public:
	SkyBox(TextureCube * texture);
	~SkyBox();

	TextureCube * GetTexture();
	IndexedMesh * GetMesh();
};