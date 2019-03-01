#pragma once
#include <EnginePch.h>
#include <Graphics/GameObject.h>
#include <Graphics/Camera.h>
#include <Graphics/Lights/DirectionalLight.h>
#include <Graphics/Lights/PointLight.h>
#include <Graphics/Lights/SpotLight.h>
#include <Graphics/SkyBox.h>
#include <Graphics/Textures/PlanarReflector.h>
#include <Graphics/Particles/ParticleEmitter.h>
#include <Graphics/Particles/MeshEmitter.h>

class API Scene
{
public:
	Scene() noexcept;
	virtual ~Scene();

	virtual void OnUpdate(float dtS) noexcept;
	
	void SetSkyBox(SkyBox* pSkyBox) noexcept;
	void SetCamera(Camera* pCamera, uint32 index = 0) noexcept;
	void SetConceal(bool conceal) noexcept;
	
	void SelectCamera(uint32 index);
	void AddGameObject(GameObject* pGameObject) noexcept;
	void AddDirectionalLight(DirectionalLight* pLight) noexcept;
	void AddPointLight(PointLight* pLight) noexcept;
	void AddSpotLight(SpotLight* pLight) noexcept;
	void AddRoomLight(PointLight* pLight) noexcept;
	void AddPlanarReflector(PlanarReflector* pReflector) noexcept;
	void RemoveSpotLight(SpotLight* pLight) noexcept;
	void RemoveGameObject(uint32 index) noexcept;
	void ExtendScene() noexcept;

	Camera& GetCamera() noexcept;
	const Camera& GetCamera() const noexcept;
	SkyBox& GetSkyBox() const noexcept;
	const GameObject* GetGameObject(const std::string& name) const noexcept;
	GameObject* GetGameObject(const std::string& name) noexcept;
	const std::vector<PlanarReflector*>& GetPlanarReflectors() const noexcept;
	std::vector<GameObject*>& GetGameObjects() noexcept;
	const std::vector<GameObject*>& GetGameObjects() const noexcept;
	const std::vector<GameObject*>& GetDrawables() const noexcept;
	const std::vector<GameObject*>& GetAnimatedDrawables() const noexcept;
	const std::vector<GameObject*>& GetDecals() const noexcept;
	const std::vector<GameObject*>& GetReflectables() const noexcept;
	const std::vector<ParticleEmitter*>& GetParticleEmitters() const noexcept;
	const std::vector<MeshEmitter*>& GetMeshEmitters() const noexcept;
	const std::vector<DirectionalLight*>& GetDirectionalLights() const noexcept;
	const std::vector<PointLight*>& GetPointLights() const noexcept;
	const std::vector<SpotLight*>& GetSpotLights() const noexcept;
	const std::vector<PointLight*>& GetRoomLights() const noexcept;
	std::vector<PointLight*>& GetRoomLights() noexcept;
	const float GetExtension() const noexcept;

private:
	Camera* m_pCamera;
	std::vector<Camera*> m_Cameras;
	std::unordered_map<std::string, GameObject*> m_NamedObjects;
	std::vector<GameObject*> m_GameObjects;
	std::vector<GameObject*> m_Drawables;
	std::vector<GameObject*> m_AnimatedDrawables;
	std::vector<GameObject*> m_Decals;
	std::vector<GameObject*> m_Reflectables;
	std::vector<ParticleEmitter*> m_ParticleEmitters;
	std::vector<MeshEmitter*> m_MeshEmitters;
	std::vector<PlanarReflector*> m_PlanarReflectors;
	std::vector<DirectionalLight*> m_DirectionalLights;
	std::vector<PointLight*> m_PointLights;
	std::vector<SpotLight*> m_SpotLights;
	SkyBox* m_pSkyBox;

	float m_Extension = 0.0f;
	bool m_Extending = false;
	bool m_Extended = false;
};

inline Camera& Scene::GetCamera() noexcept
{
	assert(m_pCamera != nullptr);
	return *m_pCamera;
}

inline const Camera& Scene::GetCamera() const noexcept
{
	assert(m_pCamera != nullptr);
	return *m_pCamera;
}

inline SkyBox& Scene::GetSkyBox() const noexcept
{
	return *m_pSkyBox;
}

inline std::vector<GameObject*>& Scene::GetGameObjects() noexcept
{
	return m_GameObjects;
}

inline const std::vector<GameObject*>& Scene::GetGameObjects() const noexcept
{
	return m_GameObjects;
}

inline const std::vector<GameObject*>& Scene::GetDrawables() const noexcept
{
	return m_Drawables;
}

inline const std::vector<GameObject*>& Scene::GetAnimatedDrawables() const noexcept
{
	return m_AnimatedDrawables;
}

inline const std::vector<GameObject*>& Scene::GetDecals() const noexcept
{
	return m_Decals;
}

inline const std::vector<GameObject*>& Scene::GetReflectables() const noexcept
{
	return m_Reflectables;
}

inline const std::vector<ParticleEmitter*>& Scene::GetParticleEmitters() const noexcept
{
	return m_ParticleEmitters;
}

inline const std::vector<MeshEmitter*>& Scene::GetMeshEmitters() const noexcept
{
	return m_MeshEmitters;
}

inline const std::vector<DirectionalLight*>& Scene::GetDirectionalLights() const noexcept
{
	return m_DirectionalLights;
}

inline const std::vector<PointLight*>& Scene::GetPointLights() const noexcept
{
	return m_PointLights;
}

inline const std::vector<SpotLight*>& Scene::GetSpotLights() const noexcept
{
	return m_SpotLights;
}

inline const std::vector<PointLight*>& Scene::GetRoomLights() const noexcept
{
	return m_RoomLights;
}

inline std::vector<PointLight*>& Scene::GetRoomLights() noexcept
{
	return m_RoomLights;
}

inline const float Scene::GetExtension() const noexcept
{
	return m_Extension;
}
