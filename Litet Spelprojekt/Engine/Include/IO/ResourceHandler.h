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
#include <IO/GAMEOBJECT.h>
#include <IO/SOUND.h>
#include <IO/MUSIC.h>

class GameObject;
class SoundEffect;
class Music;

class API ResourceHandler : public IRunnable
{
	friend class Application;

public:
	static uint32 RegisterMesh(const std::string& filename);
	static uint32 RegisterMesh(IndexedMesh* mesh);
	static uint32 RegisterTexture2D(const std::string& filename, TEX_FORMAT format, bool generateMipmaps = true, const TextureParams& params = TextureParams());
	static uint32 RegisterMaterial(uint32 texture, int32 normalMap = -1);
	static uint32 RegisterMaterial(const glm::vec4& color, int32 normalMap = -1);
	static uint32 RegisterDecal(int32 texture, int32 normalMap);
	static uint32 RegisterGameObject(std::string name, uint32 mesh, uint32 material, int32 decal = -1);
	static uint32 RegisterSound(const std::string filename);
	static uint32 RegisterMusic(const std::string filename);

	static IndexedMesh* GetMesh(int32 mesh);
	static int32 GetMesh(const IndexedMesh* mesh);
	static Texture2D* GetTexture2D(int32 texture);
	static Material* GetMaterial(int32 material);
	static int32 GetMaterial(const Material* material);
	static Decal* GetDecal(int32 decal);
	static const SoundEffect* GetSound(int32 sound);
	static const Music* GetMusic(int32 music);

	static GameObject* CreateGameObject(int32 gameObject);

	static std::string GetGameObjectName(int32 gameObject);

	static void QuaryGameObjectTypes(std::vector<std::string>& list);

	virtual void RunParallel();

private:
	static void Load();
	static void LoadResources(IResourceListener* resourceListener, std::string prePath = "");
	static void ConstructResources();
	static void ReleaseResources();
	static void TriggerOnLoading(const std::string& file, float percentage);

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
	};

	struct GAMEOBJECT_DESC_INTERNAL
	{
		std::string name = "";
		uint32 mesh;
		uint32 material;
		int32 decal;
	};

	struct SOUND_DESC_INTERNAL
	{
		std::string filename = "";
	};

	struct MUSIC_DESC_INTERNAL
	{
		std::string filename = "";
	};

	static MESH_DESC_INTERNAL m_pIndexedMeshFiles[64];
	static IndexedMesh* m_pIndexedMeshes[64];
	static uint32 m_NrOfMeshes;
	static uint32 m_NrOfMeshesLoaded;

	static TEXTURE2D_DESC_INTERNAL m_pTexture2DFiles[64];
	static Texture2D* m_pTexture2Ds[64];
	static uint32 m_NrOfTexture2D;
	static uint32 m_NrOfTexture2DLoaded;

	static Material* m_pMaterials[512];
	static uint32 m_NrOfMaterials;

	static Decal* m_pDecals[64];
	static uint32 m_NrOfDecals;

	static GAMEOBJECT_DESC_INTERNAL m_pGameObjectFiles[64];
	static uint32 m_NrOfGameObjects;

	static SOUND_DESC_INTERNAL m_pSoundFiles[64];
	static SoundEffect* m_pSounds[64];
	static uint32 m_NrOfSounds;
	static uint32 m_NrOfSoundsLoaded;

	static MUSIC_DESC_INTERNAL m_pMusicFiles[64];
	static Music* m_pMusic[64];
	static uint32 m_NrOfMusic;
	static uint32 m_NrOfMusicLoaded;

	static IResourceListener* m_ResourceListener;
	static std::string m_PrePath;

	ResourceHandler();
	static ResourceHandler* instance;
};