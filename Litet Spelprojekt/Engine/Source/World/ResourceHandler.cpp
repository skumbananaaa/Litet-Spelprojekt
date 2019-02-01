#include <EnginePch.h>
#include <World\ResourceHandler.h>

std::string ResourceHandler::m_pIndexedMeshFiles[64];
IndexedMesh* ResourceHandler::m_pIndexedMeshes[64];
uint32 ResourceHandler::m_NrOfMeshes = 0;

Texture2D_DESC ResourceHandler::m_pTexture2DFiles[64];
Texture2D* ResourceHandler::m_pTexture2Ds[64];
uint32 ResourceHandler::m_NrOfTexture2D = 0;

Material_DESC ResourceHandler::m_pMaterialFiles[64];
Material* ResourceHandler::m_pMaterials[64];
uint32 ResourceHandler::m_NrOfMaterials = 0;

Decal_DESC ResourceHandler::m_pDecalFiles[64];
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

uint32 ResourceHandler::RegisterMaterial(const std::string& texture, const std::string& normalMap)
{
	m_pMaterialFiles[m_NrOfMaterials] = { texture, glm::vec4(), normalMap, true };
	return m_NrOfMaterials++;
}

uint32 ResourceHandler::RegisterMaterial(const glm::vec4& color, const std::string& normalMap)
{
	m_pMaterialFiles[m_NrOfMaterials] = { "", color, normalMap, false };
	return m_NrOfMaterials++;
}

uint32 ResourceHandler::RegisterDecal(const std::string& texture, const std::string& normalMap)
{
	m_pDecalFiles[m_NrOfDecals] = { texture,  normalMap };
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

	for (int i = 0; i < m_NrOfMaterials; i++)
	{
		Material_DESC desc = m_pMaterialFiles[i];
		std::cout << "Creating Material"<< std::endl;
		Material* material = new Material();
		if (desc.isTexture)
		{
			material->SetTexture(FindTexture2D(desc.texture));
		}
		else
		{
			material->SetColor(desc.color);
		}
		if (!desc.normalMap.empty())
		{
			material->SetNormalMap(FindTexture2D(desc.normalMap));
		}
		m_pMaterials[i] = material;
	}

	for (int i = 0; i < m_NrOfDecals; i++)
	{
		Decal_DESC desc = m_pDecalFiles[i];
		std::cout << "Creating Decal" << std::endl;
		Decal* decal = new Decal();
		if (!desc.texture.empty())
		{
			decal->SetTexture(FindTexture2D(desc.texture));
		}
		if (!desc.normalMap.empty())
		{
			decal->SetNormalMap(FindTexture2D(desc.normalMap));
		}
		m_pDecals[i] = decal;
	}

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

Texture2D* ResourceHandler::FindTexture2D(std::string file)
{
	for (int i = 0; i < m_NrOfTexture2D; i++)
	{
		if (m_pTexture2DFiles[i].filename.compare(file))
		{
			return m_pTexture2Ds[i];
		}
		
	}
	std::cout << "ERROR! Texture [" << file << "] has not been registered!";
	return nullptr;
}
