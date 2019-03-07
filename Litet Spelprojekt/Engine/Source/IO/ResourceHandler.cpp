#include <EnginePch.h>
#include <IO\ResourceHandler.h>
#include <Graphics/GameObject.h>
#include <Audio/Music.h>
#include <Audio/SoundEffect.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Geometry/IndexedMesh.h>
#include <Graphics/Geometry/MeshParticle.h>
#include <Graphics/Materials/Material.h>
#include <Graphics/Materials/WaterOutdoorMaterial.h>
#include <Graphics/Materials/WaterIndoorMaterial.h>
#include <Graphics/Materials/WallMaterial.h>
#include <Graphics/Materials/Decal.h>

ResourceHandler::MESH_DESC_INTERNAL ResourceHandler::m_pIndexedMeshFiles[64];
IndexedMesh* ResourceHandler::m_pIndexedMeshes[64];
uint32 ResourceHandler::m_NrOfMeshes = 0;
uint32 ResourceHandler::m_NrOfMeshesLoaded = 0;

ResourceHandler::ANIMATED_MESH_DESC_INTERNAL ResourceHandler::m_pAnimatedMeshFiles[64];
AnimatedMesh* ResourceHandler::m_pAnimatedMeshes[64];
uint32 ResourceHandler::m_NrOfAnimatedMeshes = 0;
uint32 ResourceHandler::m_NrOfAnimatedMeshesLoaded = 0;

ResourceHandler::MESH_DESC_INTERNAL ResourceHandler::m_pMeshParticleFiles[64];
MeshParticle* ResourceHandler::m_pMeshParticles[64];
uint32 ResourceHandler::m_NrOfMeshParticles = 0;
uint32 ResourceHandler::m_NrOfMeshParticlesLoaded = 0;

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

ResourceHandler::SHADER_DESC_INTERNAL ResourceHandler::m_ShaderFiles[64];
ShaderProgram* ResourceHandler::m_pShaders[64];
uint32 ResourceHandler::m_NrOfShaders = 0;
uint32 ResourceHandler::m_NrOfShadersLoaded = 0;


IResourceListener* ResourceHandler::m_ResourceListener;
std::string ResourceHandler::m_PrePath;
IGameObjectCreator* ResourceHandler::m_pGameObjectCreator;

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
	SHADER::RegisterResources();

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

uint32 ResourceHandler::RegisterMeshParticle(const std::string& filename)
{
	m_pMeshParticleFiles[m_NrOfMeshParticles] = { filename };
	return m_NrOfMeshParticles++;
}

uint32 ResourceHandler::RegisterMeshParticle(MeshParticle* mesh)
{
	m_pMeshParticleFiles[m_NrOfMeshParticles] = { "" };
	m_pMeshParticles[m_NrOfMeshParticles] = mesh;
	return m_NrOfMeshParticles++;
}

uint32 ResourceHandler::RegisterAnimatedMesh(const std::string& filename)
{
	m_pAnimatedMeshFiles[m_NrOfAnimatedMeshes] = { filename };
	return m_NrOfAnimatedMeshes++;
}

uint32 ResourceHandler::RegisterTexture2D(const std::string& filename, TEX_FORMAT format, bool generateMipmaps, bool flipVertically, const TextureParams& params)
{
	m_pTexture2DFiles[m_NrOfTexture2D] = { filename, format, generateMipmaps, flipVertically, params};
	return m_NrOfTexture2D++;
}

uint32 ResourceHandler::RegisterMaterial(int32 texture, int32 shader)
{
	std::cout << "Creating Material" << std::endl;
	Material* material = new Material(shader);
	material->SetDiffuseMap(GetTexture2D(texture));
	m_pMaterials[m_NrOfMaterials] = material;
	return m_NrOfMaterials++;
}

uint32 ResourceHandler::RegisterMaterial(const glm::vec4& color, float specular, int32 shader)
{
	std::cout << "Creating Material" << std::endl;
	Material* material = new Material(shader);
	material->SetColor(color);
	material->SetSpecular(specular);
	m_pMaterials[m_NrOfMaterials] = material;
	return m_NrOfMaterials++;
}

uint32 ResourceHandler::RegisterWaterOutdoorMaterial()
{
	std::cout << "Creating Water Outdoor Material" << std::endl;
	WaterOutdoorMaterial* material = new WaterOutdoorMaterial();

	m_pMaterials[m_NrOfMaterials] = material;
	return m_NrOfMaterials++;
}

uint32 ResourceHandler::RegisterWaterIndoorMaterial()
{
	std::cout << "Creating Water Indoor Material" << std::endl;
	WaterIndoorMaterial* material = new WaterIndoorMaterial();

	m_pMaterials[m_NrOfMaterials] = material;
	return m_NrOfMaterials++;
}

uint32 ResourceHandler::RegisterWallMaterial(int32 texture, float specular)
{
	std::cout << "Creating Wall Material" << std::endl;
	Material* material = new WallMaterial();
	material->SetDiffuseMap(GetTexture2D(texture));
	material->SetSpecular(specular);
	m_pMaterials[m_NrOfMaterials] = material;
	return m_NrOfMaterials++;
}

uint32 ResourceHandler::RegisterDecal(int32 texture, int32 normalMap)
{
	std::cout << "Creating Decal" << std::endl;
	Decal* decal = new Decal();
	if (texture >= 0)
	{
		decal->SetDiffuseMap(GetTexture2D(texture));
	}
	if (normalMap >= 0)
	{
		decal->SetNormalMap(GetTexture2D(normalMap));
	}
	m_pDecals[m_NrOfDecals] = decal;
	return m_NrOfDecals++;
}

uint32 ResourceHandler::RegisterGameObject(std::string name, int32 mesh, int32 material, int32 decal)
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

uint32 ResourceHandler::RegisterShader(const std::string vertex, const std::string pixel, const ShaderDefines& defines)
{
	m_ShaderFiles[m_NrOfShaders] = { vertex, pixel, ParseDefines(defines) };
	return m_NrOfShaders++;
}

IndexedMesh* ResourceHandler::GetMesh(int32 mesh)
{
	if (mesh == -1)
	{
		return nullptr;
	}
	return m_pIndexedMeshes[mesh];
}

AnimatedMesh* ResourceHandler::GetAnimatedMesh(int32 mesh)
{
	if (mesh == -1)
	{
		return nullptr;
	}
	return m_pAnimatedMeshes[mesh];
}

MeshParticle* ResourceHandler::GetMeshParticle(int32 mesh)
{
	if (mesh == -1)
	{
		return nullptr;
	}

	return m_pMeshParticles[mesh];
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

const ShaderProgram* ResourceHandler::GetShader(int32 shader)
{
	if (shader == -1)
	{
		return nullptr;
	}
	return m_pShaders[shader];
}

GameObject* ResourceHandler::CreateGameObject(int32 gameObject)
{
	if (gameObject == -1)
	{
		return nullptr;
	}

	GAMEOBJECT_DESC_INTERNAL& desc = m_pGameObjectFiles[gameObject];

	GameObject* pGameObject = m_pGameObjectCreator->CreateGameObject(gameObject);
	if (desc.mesh != -1)
	{
		pGameObject->SetMesh(desc.mesh);
	}
	if (desc.material != -1)
	{
		pGameObject->SetMaterial(desc.material);
	}
	if (desc.decal != -1)
	{
		pGameObject->SetDecal(desc.decal);
	}
	pGameObject->SetTypeId(gameObject);
	pGameObject->SetName(desc.name + std::to_string(desc.count++));
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
	int32 nrOfFiles = 
		(m_NrOfTexture2D - m_NrOfTexture2DLoaded) + 
		(m_NrOfSounds - m_NrOfSoundsLoaded) + 
		(m_NrOfMusic - m_NrOfMusicLoaded) +
		(m_NrOfAnimatedMeshes - m_NrOfAnimatedMeshesLoaded) +
		(m_NrOfShaders - m_NrOfShadersLoaded);

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

	//Counting MeshParticles
	for (int i = m_NrOfMeshParticlesLoaded; i < m_NrOfMeshParticles; i++)
	{
		MESH_DESC_INTERNAL desc = m_pMeshParticleFiles[i];
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

	for (int i = m_NrOfAnimatedMeshesLoaded; i < m_NrOfAnimatedMeshes; i++)
	{
		ANIMATED_MESH_DESC_INTERNAL desc = m_pAnimatedMeshFiles[i];
		if (!desc.filename.empty())
		{
			std::cout << "Loading Animated Mesh: " << desc.filename << std::endl;
			TriggerOnLoading(desc.filename, currentFile++ / (float)nrOfFiles);
			m_pAnimatedMeshes[i] = AnimatedMesh::ReadColladaFile((m_PrePath + "Resources/Meshes/" + desc.filename).c_str());
		}
	}

	for (int i = m_NrOfMeshParticlesLoaded; i < m_NrOfMeshParticles; i++)
	{
		MESH_DESC_INTERNAL desc = m_pMeshParticleFiles[i];
		if (!desc.filename.empty())
		{
			std::cout << "Loading MeshParticle: " << desc.filename << std::endl;
			TriggerOnLoading(desc.filename, currentFile++ / (float)nrOfFiles);
			m_pMeshParticles[i] = MeshParticle::CreateMeshParticleFromFile((m_PrePath + "Resources/Meshes/" + desc.filename).c_str());
		}
	}

	for (int i = m_NrOfTexture2DLoaded; i < m_NrOfTexture2D; i++)
	{
		TEXTURE2D_DESC_INTERNAL desc = m_pTexture2DFiles[i];
		std::cout << "Loading Texture: " << desc.filename << std::endl;
		TriggerOnLoading(desc.filename, currentFile++ / (float)nrOfFiles);
		m_pTexture2Ds[i] = new Texture2D((m_PrePath + "Resources/Textures/" + desc.filename).c_str(), desc.format, desc.generateMipmaps, desc.flipVertically, desc.params);
	}

	for (int i = m_NrOfSoundsLoaded; i < m_NrOfSounds; i++)
	{
		SOUND_DESC_INTERNAL desc = m_pSoundFiles[i];
		std::cout << "Loading Sound: " << desc.filename << std::endl;
		TriggerOnLoading(desc.filename, currentFile++ / (float)nrOfFiles);
		m_pSounds[i] = new SoundEffect((m_PrePath + "Resources/Audio/" + desc.filename).c_str());
	}

	for (int i = m_NrOfMusicLoaded; i < m_NrOfMusic; i++)
	{
		MUSIC_DESC_INTERNAL desc = m_pMusicFiles[i];
		std::cout << "Loading Music: " << desc.filename << std::endl;
		TriggerOnLoading(desc.filename, currentFile++ / (float)nrOfFiles);
		m_pMusic[i] = new Music((m_PrePath + "Resources/Audio/Music/" + desc.filename).c_str());
	}

	for (int i = m_NrOfShadersLoaded; i < m_NrOfShaders; i++)
	{
		SHADER_DESC_INTERNAL desc = m_ShaderFiles[i];
		std::cout << "Loading Shader: " << desc.vertex << std::endl;
		TriggerOnLoading(desc.vertex, currentFile++ / (float)nrOfFiles);
		
		Shader* vertexShader = Shader::Create((m_PrePath + "Resources/Shaders/" + desc.vertex).c_str(), VERTEX_SHADER, desc.defines);
		Shader* pixelShader = nullptr;
		if (!desc.pixel.empty())
		{
			std::cout << "Loading Shader: " << desc.pixel << std::endl;
			pixelShader = Shader::Create((m_PrePath + "Resources/Shaders/" + desc.pixel).c_str(), FRAGMENT_SHADER, desc.defines);
		}

		m_pShaders[i] = ShaderProgram::Create(vertexShader, pixelShader);
	}
}

void ResourceHandler::Construct()
{
	for (int i = m_NrOfMeshesLoaded; i < m_NrOfMeshes; i++)
	{
		m_pIndexedMeshes[i]->Construct();
	}

	for (int i = m_NrOfAnimatedMeshesLoaded; i < m_NrOfAnimatedMeshes; i++)
	{
		m_pAnimatedMeshes[i]->Construct();
	}

	for (int i = m_NrOfMeshParticlesLoaded; i < m_NrOfMeshParticles; i++)
	{
		m_pMeshParticles[i]->Construct();
	}

	for (int i = m_NrOfTexture2DLoaded; i < m_NrOfTexture2D; i++)
	{
		m_pTexture2Ds[i]->Construct();
	}

	for (int i = m_NrOfShadersLoaded; i < m_NrOfShaders; i++)
	{
		m_pShaders[i]->Construct();
		m_pShaders[i]->SetDebugName(("ShaderProgram " + std::to_string(i)).c_str());
	}
}

void ResourceHandler::LoadResources(IResourceListener* resourceListener, std::string prePath, bool multiThreading)
{
	m_PrePath = prePath;

	MESH::RegisterResourcesPreLoading();
	TEXTURE::RegisterResourcesPreLoading();
	SOUND::RegisterResourcesPreLoading();
	MUSIC::RegisterResourcesPreLoading();
	SHADER::RegisterResourcesPreLoading();
	Load();
	Construct();

	m_NrOfMeshesLoaded = m_NrOfMeshes;
	m_NrOfAnimatedMeshesLoaded = m_NrOfAnimatedMeshes;
	m_NrOfTexture2DLoaded = m_NrOfTexture2D;
	m_NrOfSoundsLoaded = m_NrOfSounds;
	m_NrOfMusicLoaded = m_NrOfMusic;
	m_NrOfShadersLoaded = m_NrOfShaders;

	MATERIAL::RegisterResourcesPreLoading();
	DECAL::RegisterResourcesPreLoading();
	GAMEOBJECT::RegisterResourcesPreLoading();

	m_ResourceListener = resourceListener;


	instance = new ResourceHandler();

	if (!multiThreading)
	{
		instance->RunParallel();
	}
	else
	{
		ThreadHandler::RequestExecution(instance);
	}
}

void ResourceHandler::ConstructResources()
{
	Construct();

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

	for (int i = 0; i < m_NrOfAnimatedMeshes; i++)
	{
		ANIMATED_MESH_DESC_INTERNAL desc = m_pAnimatedMeshFiles[i];
		if (!desc.filename.empty())
		{
			std::cout << "Releasing Animated Mesh: " << desc.filename << std::endl;
		}
		Delete(m_pAnimatedMeshes[i]);
	}

	for (int i = 0; i < m_NrOfMeshParticles; i++)
	{
		MESH_DESC_INTERNAL desc = m_pMeshParticleFiles[i];
		if (!desc.filename.empty())
		{
			std::cout << "Releasing Mesh-Particle: " << desc.filename << std::endl;
		}
		Delete(m_pMeshParticles[i]);
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

	for (int i = 0; i < m_NrOfShaders; i++)
	{
		SHADER_DESC_INTERNAL desc = m_ShaderFiles[i];
		std::cout << "Releasing Shader: " << desc.vertex << std::endl;
		std::cout << "Releasing Shader: " << desc.pixel << std::endl;
		Delete(m_pShaders[i]);
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

void ResourceHandler::SetGameObjectCreator(IGameObjectCreator * creator)
{
	m_pGameObjectCreator = creator;
}
