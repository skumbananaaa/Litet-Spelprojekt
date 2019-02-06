#include <EnginePch.h>
#include <IO\ResourceHandler.h>

ResourceHandler::MESH_DESC_INTERNAL ResourceHandler::m_pIndexedMeshFiles[64];
IndexedMesh* ResourceHandler::m_pIndexedMeshes[64];
uint32 ResourceHandler::m_NrOfMeshes = 0;

ResourceHandler::TEXTURE2D_DESC_INTERNAL ResourceHandler::m_pTexture2DFiles[64];
Texture2D* ResourceHandler::m_pTexture2Ds[64];
uint32 ResourceHandler::m_NrOfTexture2D = 0;

Material* ResourceHandler::m_pMaterials[512];
uint32 ResourceHandler::m_NrOfMaterials = 0;

Decal* ResourceHandler::m_pDecals[64];
uint32 ResourceHandler::m_NrOfDecals = 0;

IResourceListener* ResourceHandler::m_ResourceListener;

ResourceHandler* ResourceHandler::instance = nullptr;

ResourceHandler::ResourceHandler()
{
	
}

void ResourceHandler::RunParallel()
{
	MESH::RegisterResources();
	TEXTURE::RegisterResources();

	for (int i = 0; i < m_NrOfMeshes; i++)
	{
		MESH_DESC_INTERNAL desc = m_pIndexedMeshFiles[i];
		if (!desc.filename.empty())
		{
			std::cout << "Loading Mesh: " << desc.filename << std::endl;
			m_pIndexedMeshes[i] = IndexedMesh::CreateIndexedMeshFromFile(("Resources/Meshes/" + desc.filename).c_str());
		}
	}

	for (int i = 0; i < m_NrOfTexture2D; i++)
	{
		TEXTURE2D_DESC_INTERNAL desc = m_pTexture2DFiles[i];
		std::cout << "Loading Texture: " << desc.filename << std::endl;
		m_pTexture2Ds[i] = new Texture2D(("Resources/Textures/" + desc.filename).c_str(), desc.format, desc.generateMipmaps, desc.params);
	}

	MATERIAL::RegisterResources();
	DECAL::RegisterResources();

	m_ResourceListener->OnResourcesLoaded();
}

uint32 ResourceHandler::RegisterMesh(const std::string& filename, bool showInEditor)
{
	m_pIndexedMeshFiles[m_NrOfMeshes] = { filename,  showInEditor };
	return m_NrOfMeshes++;
}

uint32 ResourceHandler::RegisterMesh(IndexedMesh* mesh, bool showInEditor)
{
	m_pIndexedMeshFiles[m_NrOfMeshes] = { "",  showInEditor };
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

std::string ResourceHandler::GetMeshName(uint32 mesh)
{
	return m_pIndexedMeshFiles[mesh].filename;
}

std::string ResourceHandler::GetMeshName(const IndexedMesh* mesh)
{
	for (uint32 i = 0; i < m_NrOfMeshes; i++)
	{
		if (m_pIndexedMeshes[i] == mesh)
		{
			return GetMeshName(i);
		}
	}
	return "Error!";
}

void ResourceHandler::QuaryMeshes(std::vector<MESH_DESC>& list)
{
	for (uint32 i = 0; i < m_NrOfMeshes; i++)
	{
		MESH_DESC_INTERNAL desc = m_pIndexedMeshFiles[i];
		if (!desc.filename.empty() && desc.showInEditor)
		{
			list.push_back({i, desc.filename});
		}
	}
}

void ResourceHandler::LoadResources(IResourceListener* resourceListener)
{
	if (!instance)
	{
		m_ResourceListener = resourceListener;
		instance = new ResourceHandler();

		instance->RunParallel();
		//ThreadHandler::RequestExecution(instance);
	}
}

void ResourceHandler::ReleaseResources()
{
	for (int i = 0; i < m_NrOfMeshes; i++)
	{
		MESH_DESC_INTERNAL desc = m_pIndexedMeshFiles[i];
		if (!desc.filename.empty())
		{
			std::cout << "Releasing Mesh: " << desc.filename << std::endl;
		}
		Delete(m_pIndexedMeshes[i]);
	}

	for (int i = 0; i < m_NrOfTexture2D; i++)
	{
		TEXTURE2D_DESC_INTERNAL desc = m_pTexture2DFiles[i];
		std::cout << "Releasing Texture: " << desc.filename << std::endl;
		Delete(m_pTexture2Ds[i]);
	}

	for (int i = 0; i < m_NrOfMaterials; i++)
	{
		std::cout << "Deleting Material" << std::endl;
		Delete(m_pMaterials[i]);
	}

	for (int i = 0; i < m_NrOfDecals; i++)
	{
		std::cout << "Deleting Decal" << std::endl;
		Delete(m_pDecals[i]);
	}

	Delete(instance);
}