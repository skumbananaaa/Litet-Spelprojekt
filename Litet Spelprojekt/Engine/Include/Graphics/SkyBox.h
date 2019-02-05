#pragma once
#include <EnginePch.h>
#include <IO/ResourceHandler.h>
#include <Graphics/Textures/TextureCube.h>
#include <Graphics/Geometry/IndexedMesh.h>

class API SkyBox
{
public:
	SkyBox(TextureCube* texture);
	~SkyBox();

	const TextureCube& GetTexture() const noexcept;
	const IndexedMesh& GetMesh() const noexcept;

private:
	IndexedMesh* m_pMesh;
	TextureCube* m_pTextureCube;
};