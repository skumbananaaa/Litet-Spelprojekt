#include <EnginePch.h>
#include <IO\ResourceHandler.h>

std::string ResourceHandler::m_pIndexedMeshFiles[64];
IndexedMesh* ResourceHandler::m_pIndexedMeshes[64];
uint32 ResourceHandler::m_NrOfMeshes = 0;

Texture2D_DESC ResourceHandler::m_pTexture2DFiles[64];
Texture2D* ResourceHandler::m_pTexture2Ds[64];
uint32 ResourceHandler::m_NrOfTexture2D = 0;

Material* ResourceHandler::m_pMaterials[64];
uint32 ResourceHandler::m_NrOfMaterials = 0;

Decal* ResourceHandler::m_pDecals[64];
uint32 ResourceHandler::m_NrOfDecals = 0;

IResourceListener* ResourceHandler::m_ResourceListener;


uint32 ResourceHandler::RegisterMesh(const std::string& filename)
{
	m_pIndexedMeshFiles[m_NrOfMeshes] = filename;
	return m_NrOfMeshes++;
}

uint32 ResourceHandler::RegisterMesh(IndexedMesh* mesh)
{
	m_pIndexedMeshes[m_NrOfMeshes] = mesh;
	return m_NrOfMeshes++;
}

uint32 ResourceHandler::RegisterTexture2D(const std::string& filename, TEX_FORMAT format, bool generateMipmaps, const TextureParams& params)
{
	m_pTexture2DFiles[m_NrOfTexture2D] = { filename, format, generateMipmaps, params};
	return m_NrOfTexture2D++;
}

uint32 ResourceHandler::RegisterMaterial(int32 texture, int32 normalMap)
{
	std::cout << "Creating Material" << std::endl;
	Material* material = new Material();
	material->SetTexture(GetTexture2D(texture));
	if (normalMap >= 0)
	{
		material->SetNormalMap(GetTexture2D(normalMap));
	}
	m_pMaterials[m_NrOfMaterials] = material;
	return m_NrOfMaterials++;
}

uint32 ResourceHandler::RegisterMaterial(const glm::vec4& color, int32 normalMap)
{
	std::cout << "Creating Material" << std::endl;
	Material* material = new Material();
	material->SetColor(color);
	if (normalMap >= 0)
	{
		material->SetNormalMap(GetTexture2D(normalMap));
	}
	m_pMaterials[m_NrOfMaterials] = material;
	return m_NrOfMaterials++;
}

uint32 ResourceHandler::RegisterDecal(int32 texture, int32 normalMap)
{
	std::cout << "Creating Decal" << std::endl;
	Decal* decal = new Decal();
	if (texture >= 0)
	{
		decal->SetTexture(GetTexture2D(texture));
	}
	if (normalMap >= 0)
	{
		decal->SetNormalMap(GetTexture2D(normalMap));
	}
	m_pDecals[m_NrOfDecals] = decal;
	return m_NrOfDecals++;
}

IndexedMesh* ResourceHandler::GetMesh(uint32 mesh)
{
	return m_pIndexedMeshes[mesh];
}

Texture2D* ResourceHandler::GetTexture2D(uint32 texture)
{
	return m_pTexture2Ds[texture];
}

Material* ResourceHandler::GetMaterial(uint32 material)
{
	return m_pMaterials[material];
}

Decal* ResourceHandler::GetDecal(uint32 decal)
{
	return m_pDecals[decal];
}

void ResourceHandler::LoadResources(IResourceListener* resourceListener)
{
	MESH::RegisterResources();
	TEXTURE::RegisterResources();

	m_ResourceListener = resourceListener;

	for (int i = 0; i < m_NrOfMeshes; i++)
	{
		if (!m_pIndexedMeshFiles[i].empty())
		{
			std::cout << "Loading Mesh: " << m_pIndexedMeshFiles[i] << std::endl;
			m_pIndexedMeshes[i] = IndexedMesh::CreateIndexedMeshFromFile(("Resources/Meshes/" + m_pIndexedMeshFiles[i]).c_str());
		}
	}

	for (int i = 0; i < m_NrOfTexture2D; i++)
	{
		Texture2D_DESC desc = m_pTexture2DFiles[i];
		std::cout << "Loading Texture: " << desc.filename << std::endl;
		m_pTexture2Ds[i] = new Texture2D(("Resources/Textures/" + desc.filename).c_str(), desc.format, desc.generateMipmaps, desc.params);
	}

	MATERIAL::RegisterResources();
	DECAL::RegisterResources();

	resourceListener->OnResourcesLoaded();
}

void ResourceHandler::ReleaseResources()
{
	for (int i = 0; i < m_NrOfMeshes; i++)
	{
		std::cout << "Releasing Mesh: " << m_pIndexedMeshFiles[i] << std::endl;
		Delete(m_pIndexedMeshes[i]);
	}

	for (int i = 0; i < m_NrOfTexture2D; i++)
	{
		Texture2D_DESC desc = m_pTexture2DFiles[i];
		std::cout << "Releasing Texture: " << desc.filename << std::endl;
		Delete(m_pTexture2Ds[i]);
	}

	for (int i = 0; i < m_NrOfMaterials; i++)
	{
		std::cout << "Deleting Material" << std::endl;
		Delete(m_pMaterials[i]);
	}
}