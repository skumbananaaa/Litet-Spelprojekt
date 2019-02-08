#include <EnginePch.h>
#include <IO\ResourceHandler.h>
#include <Graphics/GameObject.h>
#include <Audio/Music.h>
#include <Audio/SoundEffect.h>

ResourceHandler::MESH_DESC_INTERNAL ResourceHandler::m_pIndexedMeshFiles[64];
IndexedMesh* ResourceHandler::m_pIndexedMeshes[64];
uint32 ResourceHandler::m_NrOfMeshes = 0;
uint32 ResourceHandler::m_NrOfMeshesLoaded = 0;

ResourceHandler::TEXTURE2D_DESC_INTERNAL ResourceHandler::m_pTexture2DFiles[64];
Texture2D* ResourceHandler::m_pTexture2Ds[64];
uint32 ResourceHandler::m_NrOfTexture2D = 0;
uint32 ResourceHandler::m_NrOfTexture2DLoaded = 0;

Material* ResourceHandler::m_pMaterials[512];
uint32 ResourceHandler::m_NrOfMaterials = 0;

Decal* ResourceHandler::m_pDecals[64];
uint32 ResourceHandler::m_NrOfDecals = 0;

ResourceHandler::GAMEOBJECT_DESC_INTERNAL ResourceHandler::m_pGameObjectFiles[64];
uint32 ResourceHandler::m_NrOfGameObjects;

ResourceHandler::SOUND_DESC_INTERNAL ResourceHandler::m_pSoundFiles[64];
SoundEffect* ResourceHandler::m_pSounds[64];
uint32 ResourceHandler::m_NrOfSounds = 0;
uint32 ResourceHandler::m_NrOfSoundsLoaded = 0;

ResourceHandler::MUSIC_DESC_INTERNAL ResourceHandler::m_pMusicFiles[64];
Music* ResourceHandler::m_pMusic[64];
uint32 ResourceHandler::m_NrOfMusic = 0;
uint32 ResourceHandler::m_NrOfMusicLoaded = 0;


IResourceListener* ResourceHandler::m_ResourceListener;
std::string ResourceHandler::m_PrePath;

ResourceHandler* ResourceHandler::instance = nullptr;

ResourceHandler::ResourceHandler()
{
	
}

void ResourceHandler::RunParallel()
{
	MESH::RegisterResources();
	TEXTURE::RegisterResources();
	SOUND::RegisterResources();
	MUSIC::RegisterResources();

	Load();

	m_ResourceListener->OnResourceLoadingFinished();
}

uint32 ResourceHandler::RegisterMesh(const std::string& filename)
{
	m_pIndexedMeshFiles[m_NrOfMeshes] = { filename };
	return m_NrOfMeshes++;
}

uint32 ResourceHandler::RegisterMesh(IndexedMesh* mesh)
{
	m_pIndexedMeshFiles[m_NrOfMeshes] = { "" };
	m_pIndexedMeshes[m_NrOfMeshes] = mesh;
	return m_NrOfMeshes++;
}

uint32 ResourceHandler::RegisterTexture2D(const std::string& filename, TEX_FORMAT format, bool generateMipmaps, const TextureParams& params)
{
	m_pTexture2DFiles[m_NrOfTexture2D] = { filename, format, generateMipmaps, params};
	return m_NrOfTexture2D++;
}

uint32 ResourceHandler::RegisterMaterial(uint32 texture, int32 normalMap)
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

uint32 ResourceHandler::RegisterGameObject(std::string name, uint32 mesh, uint32 material, int32 decal)
{
	std::cout << "Creating GameObject" << std::endl;
	m_pGameObjectFiles[m_NrOfGameObjects] = { name,  mesh,  material, decal };
	return m_NrOfGameObjects++;
}

uint32 ResourceHandler::RegisterSound(const std::string filename)
{
	m_pSoundFiles[m_NrOfSounds] = { filename };
	return m_NrOfSounds++;
}

uint32 ResourceHandler::RegisterMusic(const std::string filename)
{
	m_pMusicFiles[m_NrOfMusic] = { filename };
	return m_NrOfMusic++;
}

IndexedMesh* ResourceHandler::GetMesh(int32 mesh)
{
	if (mesh == -1)
	{
		return nullptr;
	}
	return m_pIndexedMeshes[mesh];
}

int32 ResourceHandler::GetMesh(const IndexedMesh* mesh)
{
	for (uint32 i = 0; i < m_NrOfMeshes; i++)
	{
		if (m_pIndexedMeshes[i] == mesh)
		{
			return i;
		}
	}
	return -1;
}

Texture2D* ResourceHandler::GetTexture2D(int32 texture)
{
	if (texture == -1)
	{
		return nullptr;
	}
	return m_pTexture2Ds[texture];
}

Material* ResourceHandler::GetMaterial(int32 material)
{
	if (material == -1)
	{
		return nullptr;
	}
	return m_pMaterials[material];
}

int32 ResourceHandler::GetMaterial(const Material* material)
{
	for (uint32 i = 0; i < m_NrOfMaterials; i++)
	{
		if (m_pMaterials[i] == material)
		{
			return i;
		}
	}
	return -1;
}

Decal* ResourceHandler::GetDecal(int32 decal)
{
	if (decal == -1)
	{
		return nullptr;
	}
	return m_pDecals[decal];
}

const SoundEffect* ResourceHandler::GetSound(int32 sound)
{
	if (sound == -1)
	{
		return nullptr;
	}
	return m_pSounds[sound];
}

const Music* ResourceHandler::GetMusic(int32 music)
{
	if (music == -1)
	{
		return nullptr;
	}
	return m_pMusic[music];
}

GameObject* ResourceHandler::CreateGameObject(int32 gameObject)
{
	if (gameObject == -1)
	{
		return nullptr;
	}
	GAMEOBJECT_DESC_INTERNAL desc = m_pGameObjectFiles[gameObject];
	GameObject* pGameObject = new GameObject();
	pGameObject->SetMesh(desc.mesh);
	pGameObject->SetMaterial(desc.material);
	pGameObject->SetDecal(desc.decal);
	pGameObject->SetTypeId(gameObject);
	return pGameObject;
}

std::string ResourceHandler::GetGameObjectName(int32 gameObject)
{
	if (gameObject == -1)
	{
		return "ERROR!";
	}
	return m_pGameObjectFiles[gameObject].name;
}

void ResourceHandler::QuaryGameObjectTypes(std::vector<std::string>& list)
{
	for (uint32 i = 0; i < m_NrOfGameObjects; i++)
	{
		list.push_back(m_pGameObjectFiles[i].name);
	}
}

void ResourceHandler::Load()
{
	int32 nrOfFiles = (m_NrOfTexture2D - m_NrOfTexture2DLoaded) + (m_NrOfSounds - m_NrOfSoundsLoaded) + (m_NrOfMusic - m_NrOfMusicLoaded);
	int32 currentFile = 1;

	//Counting Meshes
	for (int i = m_NrOfMeshesLoaded; i < m_NrOfMeshes; i++)
	{
		MESH_DESC_INTERNAL desc = m_pIndexedMeshFiles[i];
		if (!desc.filename.empty())
		{
			nrOfFiles++;
		}
	}

	for (int i = m_NrOfMeshesLoaded; i < m_NrOfMeshes; i++)
	{
		MESH_DESC_INTERNAL desc = m_pIndexedMeshFiles[i];
		if (!desc.filename.empty())
		{
			std::cout << "Loading Mesh: " << desc.filename << std::endl;
			TriggerOnLoading(desc.filename, currentFile++ / (float)nrOfFiles);
			m_pIndexedMeshes[i] = IndexedMesh::CreateIndexedMeshFromFile((m_PrePath + "Resources/Meshes/" + desc.filename).c_str());
		}
	}
	m_NrOfMeshesLoaded = m_NrOfMeshes;


	for (int i = m_NrOfTexture2DLoaded; i < m_NrOfTexture2D; i++)
	{
		TEXTURE2D_DESC_INTERNAL desc = m_pTexture2DFiles[i];
		std::cout << "Loading Texture: " << desc.filename << std::endl;
		TriggerOnLoading(desc.filename, currentFile++ / (float)nrOfFiles);
		m_pTexture2Ds[i] = new Texture2D((m_PrePath + "Resources/Textures/" + desc.filename).c_str(), desc.format, desc.generateMipmaps, desc.params);
	}
	m_NrOfTexture2DLoaded = m_NrOfTexture2D;


	for (int i = m_NrOfSoundsLoaded; i < m_NrOfSounds; i++)
	{
		SOUND_DESC_INTERNAL desc = m_pSoundFiles[i];
		std::cout << "Loading Sound: " << desc.filename << std::endl;
		TriggerOnLoading(desc.filename, currentFile++ / (float)nrOfFiles);
		m_pSounds[i] = new SoundEffect((m_PrePath + "Resources/Audio/" + desc.filename).c_str());
	}
	m_NrOfSoundsLoaded = m_NrOfSounds;


	for (int i = m_NrOfMusicLoaded; i < m_NrOfMusic; i++)
	{
		MUSIC_DESC_INTERNAL desc = m_pMusicFiles[i];
		std::cout << "Loading Music: " << desc.filename << std::endl;
		TriggerOnLoading(desc.filename, currentFile++ / (float)nrOfFiles);
		m_pMusic[i] = new Music((m_PrePath + "Resources/Audio/Music/" + desc.filename).c_str());
	}
	m_NrOfMusicLoaded = m_NrOfMusic;
}

void ResourceHandler::LoadResources(IResourceListener* resourceListener, std::string prePath)
{
	m_PrePath = prePath;

	MESH::RegisterResourcesPreLoading();
	TEXTURE::RegisterResourcesPreLoading();
	SOUND::RegisterResourcesPreLoading();
	MUSIC::RegisterResourcesPreLoading();
	Load();
	MATERIAL::RegisterResourcesPreLoading();
	DECAL::RegisterResourcesPreLoading();
	GAMEOBJECT::RegisterResourcesPreLoading();

	m_ResourceListener = resourceListener;


	instance = new ResourceHandler();
	//instance->RunParallel();
	ThreadHandler::RequestExecution(instance);
}

void ResourceHandler::ConstructResources()
{
	for (int i = 0; i < m_NrOfMeshes; i++)
	{
		m_pIndexedMeshes[i]->Construct();
	}

	for (int i = 0; i < m_NrOfTexture2D; i++)
	{
		m_pTexture2Ds[i]->Construct();
	}

	MATERIAL::RegisterResources();
	DECAL::RegisterResources();
	GAMEOBJECT::RegisterResources();
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

	for (int i = 0; i < m_NrOfSounds; i++)
	{
		SOUND_DESC_INTERNAL desc = m_pSoundFiles[i];
		std::cout << "Releasing Sound: " << desc.filename << std::endl;
		Delete(m_pSounds[i]);
	}

	for (int i = 0; i < m_NrOfMusic; i++)
	{
		MUSIC_DESC_INTERNAL desc = m_pMusicFiles[i];
		std::cout << "Releasing Music: " << desc.filename << std::endl;
		Delete(m_pMusic[i]);
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

void ResourceHandler::TriggerOnLoading(const std::string& file, float percentage)
{
	if (m_ResourceListener)
	{
		m_ResourceListener->OnLoading(file, percentage);
	}
}
