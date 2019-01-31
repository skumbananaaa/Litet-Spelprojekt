#pragma once
#include "Textures/TextureCube.h"
#include "Geometry/IndexedMesh.h"
class API SkyBox
{
private:
	IndexedMesh * m_pMesh;
	TextureCube * m_pTextureCube;
public:
	SkyBox(TextureCube * texture);
	~SkyBox();

	TextureCube& GetTexture() const noexcept;
	IndexedMesh& GetMesh() const noexcept;
};