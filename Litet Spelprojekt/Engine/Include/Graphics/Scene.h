#pragma once
#include <EnginePch.h>
#include <Graphics/GameObject.h>
#include <Graphics/Camera.h>
#include <Graphics/Lights/DirectionalLight.h>
#include <Graphics/Lights/PointLight.h>
#include <Graphics/Lights/SpotLight.h>
#include <Graphics/SkyBox.h>
#include <Graphics/Textures/PlanarReflector.h>
#include <World/World.h>

class API Scene
{
public:
	Scene() noexcept;
	~Scene();

	void OnUpdate(float dtS) noexcept;
	
	void SetCamera(Camera* pCamera, uint32 index = 0) noexcept;
	void SelectCamera(uint32 index);
	void SetSkyBox(SkyBox* pSkyBox) noexcept;
	void SetWorld(World* pWorld) noexcept;
	void AddGameObject(GameObject* pGameObject) noexcept;
	void AddDirectionalLight(DirectionalLight* pLight) noexcept;
	void AddPointLight(PointLight* pLight) noexcept;
	void AddSpotLight(SpotLight* pLight) noexcept;
	void AddRoomLight(PointLight* pLight) noexcept;
	void AddPlanarReflector(PlanarReflector* pReflector) noexcept;
	void RemoveGameObject(uint32 index) noexcept;
	void ExtendScene(bool extend) noexcept;
	void SetConceal(bool conceal) noexcept;
	void ViewRoom(uint32 room) noexcept;

	Camera& GetCamera() noexcept;
	const Camera& GetCamera() const noexcept;
	SkyBox& GetSkyBox() const noexcept;

	const GameObject* GetGameObject(const std::string& name) const noexcept;
	GameObject* GetGameObject(const std::string& name) noexcept;
	const std::vector<PlanarReflector*>& GetPlanarReflectors() const noexcept;
	std::vector<GameObject*>& GetGameObjects() noexcept;
	const std::vector<GameObject*>& GetGameObjects() const noexcept;
	const std::vector<GameObject*>& GetDrawables() const noexcept;
	const std::vector<GameObject*>& GetDecals() const noexcept;
	const std::vector<GameObject*>& GetReflectables() const noexcept;
	const std::vector<DirectionalLight*>& GetDirectionalLights() const noexcept;
	const std::vector<PointLight*>& GetPointLights() const noexcept;
	const std::vector<SpotLight*>& GetSpotLights() const noexcept;
	const World* GetWorld() const noexcept;
	World* GetWorld() noexcept;
	const bool IsExtended() const noexcept;
	const bool IsConcealed() const noexcept;

private:
	Camera* m_pCamera;
	std::vector<Camera*> m_Cameras;
	std::unordered_map<std::string, GameObject*> m_NamedObjects;
	std::vector<GameObject*> m_GameObjects;
	std::vector<GameObject*> m_Drawables;
	std::vector<GameObject*> m_Decals;
	std::vector<GameObject*> m_Reflectables;
	std::vector<PlanarReflector*> m_PlanarReflectors;
	std::vector<DirectionalLight*> m_DirectionalLights;
	std::vector<PointLight*> m_PointLights;
	std::vector<SpotLight*> m_SpotLights;
	std::vector<PointLight*> m_RoomLights;
	std::vector<float> m_LightTimer;
	std::vector<uint32> m_ActiveRooms;
	SkyBox* m_pSkyBox;
	World* m_pWorld;

	uint32 m_CurrentLight = 0;
	bool m_Concealed = false;
	bool m_Extended = false;
	bool m_WasExtended = false;
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

inline const std::vector<GameObject*>& Scene::GetDecals() const noexcept
{
	return m_Decals;
}

inline const std::vector<GameObject*>& Scene::GetReflectables() const noexcept
{
	return m_Reflectables;
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

inline const World * Scene::GetWorld() const noexcept
{
	return m_pWorld;
}

inline World * Scene::GetWorld() noexcept
{
	return m_pWorld;
}

inline const bool Scene::IsExtended() const noexcept
{
	return m_Extended;
}

inline const bool Scene::IsConcealed() const noexcept
{
	return m_Concealed;
}
