#pragma once
#include <EnginePch.h>
#include <Graphics/Geometry/IndexedMesh.h>
#include <Graphics/Materials/Material.h>
#include <Graphics/Materials/Decal.h>
#include <IO/IResourceListener.h>

#include <System/ThreadHandler.h>

#include <IO/MESH.h>
#include <IO/TEXTURE.h>
#include <IO/MATERIAL.h>
#include <IO/DECAL.h>

class API ResourceHandler : public IRunnable
{
public:
	static uint32 RegisterMesh(const std::string& filename, bool showInEditor = true);
	static uint32 RegisterMesh(IndexedMesh* mesh, bool showInEditor = true);
	static uint32 RegisterTexture2D(const std::string& filename, TEX_FORMAT format, bool generateMipmaps = true, const TextureParams& params = TextureParams());
	static uint32 RegisterMaterial(int32 texture, int32 normalMap = -1);
	static uint32 RegisterMaterial(const glm::vec4& color, int32 normalMap = -1);
	static uint32 RegisterDecal(int32 texture, int32 normalMap);

	static IndexedMesh* GetMesh(uint32 mesh);
	static Texture2D* GetTexture2D(uint32 texture);
	static Material* GetMaterial(uint32 material);
	static Decal* GetDecal(uint32 decal);

	static std::string GetMeshName(uint32 mesh);
	static std::string GetMeshName(const IndexedMesh* mesh);

	static void QuaryMeshes(std::vector<MESH_DESC>& list);

	static void LoadResources(IResourceListener* resourceListener);
	static void ReleaseResources();

	virtual void RunParallel();

private:
	struct TEXTURE2D_DESC_INTERNAL
	{
		std::string filename = "";
		TEX_FORMAT format;
		bool generateMipmaps;
		TextureParams params;
	};

	struct MESH_DESC_INTERNAL
	{
		std::string filename = "";
		bool showInEditor = false;
	};

	static MESH_DESC_INTERNAL m_pIndexedMeshFiles[64];
	static IndexedMesh* m_pIndexedMeshes[64];
	static uint32 m_NrOfMeshes;

	static TEXTURE2D_DESC_INTERNAL m_pTexture2DFiles[64];
	static Texture2D* m_pTexture2Ds[64];
	static uint32 m_NrOfTexture2D;

	static Material* m_pMaterials[512];
	static uint32 m_NrOfMaterials;

	static Decal* m_pDecals[64];
	static uint32 m_NrOfDecals;

	static IResourceListener* m_ResourceListener;

	ResourceHandler();
	static ResourceHandler* instance;
};