#pragma once
#include <EnginePch.h>
#include <Graphics/Geometry/IndexedMesh.h>
#include <Graphics/Materials/Material.h>
#include <Graphics/Materials/Decal.h>
#include <World/IResourceListener.h>

struct Texture2D_DESC
{
	std::string filename;
	TEX_FORMAT format;
	bool generateMipmaps;
	TextureParams params;
};

struct Material_DESC
{
	std::string texture;
	glm::vec4 color;
	std::string normalMap;
	bool isTexture;
};

struct Decal_DESC
{
	std::string texture;
	std::string normalMap;
};

class API ResourceHandler
{
public:
	static uint32 RegisterMesh(const std::string& filename);
	static uint32 RegisterMesh(IndexedMesh* mesh);
	static uint32 RegisterTexture2D(const std::string& filename, TEX_FORMAT format, bool generateMipmaps = true, const TextureParams& params = TextureParams());
	static uint32 RegisterMaterial(const std::string& texture, const std::string& normalMap = "");
	static uint32 RegisterMaterial(const glm::vec4& color, const std::string& normalMap = "");
	static uint32 RegisterDecal(const std::string& texture, const std::string& normalMap);

	static IndexedMesh* GetMesh(uint32 mesh);
	static Texture2D* GetTexture2D(uint32 texture);
	static Material* GetMaterial(uint32 material);
	static Decal* GetDecal(uint32 decal);

	static void LoadResources(IResourceListener* resourceListener);
	static void ReleaseResources();

private:

	static Texture2D* FindTexture2D(std::string file);

	static std::string m_pIndexedMeshFiles[64];
	static IndexedMesh* m_pIndexedMeshes[64];
	static uint32 m_NrOfMeshes;

	static Texture2D_DESC m_pTexture2DFiles[64];
	static Texture2D* m_pTexture2Ds[64];
	static uint32 m_NrOfTexture2D;

	static Material_DESC m_pMaterialFiles[64];
	static Material* m_pMaterials[64];
	static uint32 m_NrOfMaterials;

	static Decal_DESC m_pDecalFiles[64];
	static Decal* m_pDecals[64];
	static uint32 m_NrOfDecals;

	static IResourceListener* m_ResourceListener;
};