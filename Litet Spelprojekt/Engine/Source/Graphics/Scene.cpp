#include <EnginePch.h>
#include <Graphics\Scene.h>

Scene::Scene() noexcept
{
	m_pCamera = nullptr;
	m_pSkyBox = nullptr;
}

Scene::~Scene()
{
	for (size_t i = 0; i < m_Cameras.size(); i++)
	{
		DeleteSafe(m_Cameras[i]);
	}

	DeleteSafe(m_pSkyBox);

	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		DeleteSafe(m_GameObjects[i]);
	}

	for (size_t i = 0; i < m_DirectionalLights.size(); i++)
	{
		DeleteSafe(m_DirectionalLights[i]);
	}

	for (size_t i = 0; i < m_PointLights.size(); i++)
	{
		DeleteSafe(m_PointLights[i]);
	}

	for (size_t i = 0; i < m_SpotLights.size(); i++)
	{
		DeleteSafe(m_SpotLights[i]);
	}

	for (size_t i = 0; i < m_PlanarReflectors.size(); i++)
	{
		DeleteSafe(m_PlanarReflectors[i]);
	}

	for (size_t i = 0; i < m_ParticleSystems.size(); i++)
	{
		DeleteSafe(m_ParticleSystems[i]);
	}
}

void Scene::SetCamera(Camera* pCamera, uint32 index) noexcept
{
	if (m_Cameras.size() == index)
	{
		m_Cameras.push_back(pCamera);
	}
	else if (index > m_Cameras.size())
	{
		std::cout << "Failed to set camera!!! Index out of range" << std::endl;
		return;
	}
	else
	{
		m_Cameras[index] = pCamera;
	}
	m_pCamera = pCamera;
}

void Scene::SelectCamera(uint32 index)
{
	m_pCamera = m_Cameras[index];
}

void Scene::SetSkyBox(SkyBox* pSkyBox) noexcept
{
	m_pSkyBox = pSkyBox;
}

const GameObject* Scene::GetGameObject(const std::string& name) const noexcept
{
	auto item = m_NamedObjects.find(name);
	if (item == m_NamedObjects.end())
	{
		return nullptr;
	}

	return item->second;
}

GameObject* Scene::GetGameObject(const std::string& name) noexcept
{
	auto item = m_NamedObjects.find(name);
	if (item == m_NamedObjects.end())
	{
		return nullptr;
	}

	return item->second;
}

const std::vector<PlanarReflector*>& Scene::GetPlanarReflectors() const noexcept
{
	return m_PlanarReflectors;
}

void Scene::AddGameObject(GameObject* pGameObject) noexcept
{
	assert(pGameObject != nullptr);

	pGameObject->OnAddedToScene(this);
	m_GameObjects.push_back(pGameObject);
	if (pGameObject->HasMaterial() && pGameObject->HasMesh())
	{
		m_Drawables.push_back(pGameObject);
	}
	
	if (pGameObject->HasDecal())
	{
		m_Decals.push_back(pGameObject);
	}

	if (pGameObject->IsReflectable())
	{
		m_Reflectables.push_back(pGameObject);
	}

	const std::string& name = pGameObject->GetName();
	if (name != "")
	{
		m_NamedObjects[name] = pGameObject;
	}
}

void Scene::AddParticleSystem(ParticleSystem* pParticleSystem) noexcept
{
	m_ParticleSystems.push_back(pParticleSystem);
}

void Scene::AddDirectionalLight(DirectionalLight* pLight) noexcept
{
	m_DirectionalLights.push_back(pLight);
}

void Scene::AddPointLight(PointLight* pLight) noexcept
{
	m_PointLights.push_back(pLight);
}

void Scene::AddSpotLight(SpotLight* pLight) noexcept
{
	m_SpotLights.push_back(pLight);
}

void Scene::AddRoomLight(PointLight* pLight) noexcept
{
	m_RoomLights.push_back(pLight);
	AddPointLight(pLight);
}

void Scene::RemoveSpotLight(SpotLight* pLight) noexcept
{
	int32 counter = 0;
	for (SpotLight* object : m_SpotLights)
	{
		if (object == pLight)
		{
			m_SpotLights.erase(m_SpotLights.begin() + counter);
			return;
		}
		counter++;
	}
}

void Scene::AddPlanarReflector(PlanarReflector* pReflector) noexcept
{
	m_PlanarReflectors.push_back(pReflector);
}

void Scene::RemoveGameObject(uint32 index) noexcept
{
	m_GameObjects.erase(m_GameObjects.begin() + index);
}

void Scene::ExtendScene() noexcept
{
	m_Extending = true;
	m_Extended = !m_Extended;
}

void Scene::OnUpdate(float dtS) noexcept
{
	for (GameObject* pGameObject : m_GameObjects)
	{
		pGameObject->Update(dtS);
	}

	for (SpotLight* pSpotLight : m_SpotLights)
	{
		pSpotLight->Update(dtS);
	}
	for (PointLight* pPointLight : m_PointLights)
	{
		pPointLight->Update(dtS);
	}

	for (ParticleSystem* pSystem : m_ParticleSystems)
	{
		pSystem->Update(*m_pCamera, dtS);
	}

	if (m_Extending)
	{
		m_Extension += 20.0f * dtS * ((m_Extended * 2) - 1);

		if (m_Extension > 10.0f)
		{
			m_Extending = false;
			m_Extension = 10.0f;
		}
		else if (m_Extension < 0.0f)
		{
			m_Extending = false;
			m_Extension = 0.0f;
		}
	}
}
