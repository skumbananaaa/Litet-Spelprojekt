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

	for (size_t i = 0; i < m_AnimatedDrawables.size(); i++)
	{
		ThreadHandler::RemoveMultiUpdater(m_AnimatedDrawables[i]);
	}

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

	//Add gameobjects that should be called in onupdate
	if (pGameObject->IsTickable())
	{
		m_UpdateAbles.push_back(pGameObject);
	}

	//Add gameobjects that has a mesh and material aka is drawable
	if (pGameObject->HasMaterial() && pGameObject->HasMesh())
	{
		m_Drawables.push_back(pGameObject);
	}

	//Add gameobjects that has a animatedmesh and material aka is animateddrawable
	if (pGameObject->HasAnimatedMesh() && pGameObject->HasMaterial())
	{
		m_AnimatedDrawables.push_back(pGameObject);
		ThreadHandler::AddMultiUpdater(pGameObject);
	}
	
	//Add gameobjects that has a decal
	//if (pGameObject->HasDecal())
	//{
	//	m_Decals.push_back(pGameObject);
	//}

	//Add gameobjects that has a reflectable material
	if (pGameObject->HasMaterial())
	{
		if (pGameObject->GetMaterial()->IsReflectable())
		{
			m_Reflectables.push_back(pGameObject);
		}
	}

	//Add particleemitters
	ParticleEmitter* pEmitter = dynamic_cast<ParticleEmitter*>(pGameObject);
	if (pEmitter != nullptr)
	{
		m_ParticleEmitters.push_back(pEmitter);
	}

	//Add meshemitters
	MeshEmitter* pMeshEmitter = dynamic_cast<MeshEmitter*>(pGameObject);
	if (pMeshEmitter != nullptr)
	{
		m_MeshEmitters.push_back(pMeshEmitter);
	}

	//Add named objects into a hashtable
	const std::string& name = pGameObject->GetName();
	if (name != "")
	{
		m_NamedObjects[name] = pGameObject;
	}
}

void Scene::AddDirectionalLight(DirectionalLight* pLight) noexcept
{
	if (pLight->IsTickable())
	{
		m_UpdateAbles.push_back(pLight);
	}

	m_DirectionalLights.push_back(pLight);
}

void Scene::AddPointLight(PointLight* pLight) noexcept
{
	if (pLight->IsTickable())
	{
		m_UpdateAbles.push_back(pLight);
	}

	m_PointLights.push_back(pLight);
}

void Scene::AddSpotLight(SpotLight* pLight) noexcept
{
	if (pLight->IsTickable())
	{
		m_UpdateAbles.push_back(pLight);
	}

	m_SpotLights.push_back(pLight);
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

void Scene::RemoveGameObject(GameObject* pGameObject) noexcept
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		if (m_GameObjects[i] == pGameObject)
		{
			m_GameObjects.erase(m_GameObjects.begin() + i);
			break;
		}
	}

	if (pGameObject->HasMaterial() && pGameObject->HasMesh())
	{
		for (int i = 0; i < m_Drawables.size(); i++)
		{
			if (m_Drawables[i] == pGameObject)
			{
				m_Drawables.erase(m_Drawables.begin() + i);
				break;
			}
		}
	}

	if (pGameObject->HasAnimatedMesh() && pGameObject->HasMaterial())
	{
		for (int i = 0; i < m_AnimatedDrawables.size(); i++)
		{
			if (m_AnimatedDrawables[i] == pGameObject)
			{
				ThreadHandler::RemoveMultiUpdater(pGameObject);
				m_AnimatedDrawables.erase(m_AnimatedDrawables.begin() + i);
				break;
			}
		}
	}

	if (pGameObject->HasDecal())
	{
		for (int i = 0; i < m_Decals.size(); i++)
		{
			if (m_Decals[i] == pGameObject)
			{
				m_Decals.erase(m_Decals.begin() + i);
				break;
			}
		}
	}

	if (pGameObject->HasMaterial())
	{
		if (pGameObject->GetMaterial()->IsReflectable())
		{
			for (int i = 0; i < m_Reflectables.size(); i++)
			{
				if (m_Reflectables[i] == pGameObject)
				{
					m_Reflectables.erase(m_Reflectables.begin() + i);
					break;
				}
			}
		}
	}

	if (pGameObject->IsTickable())
	{
		for (int i = 0; i < m_UpdateAbles.size(); i++)
		{
			if (m_UpdateAbles[i] == pGameObject)
			{
				m_UpdateAbles.erase(m_UpdateAbles.begin() + i);
				break;
			}
		}

		//std::sort(m_UpdateAbles.begin(), m_UpdateAbles.end());
	}

	ParticleEmitter* pEmitter = dynamic_cast<ParticleEmitter*>(pGameObject);
	if (pEmitter != nullptr)
	{
		for (int i = 0; i < m_ParticleEmitters.size(); i++)
		{
			if (m_ParticleEmitters[i] == pGameObject)
			{
				m_ParticleEmitters.erase(m_ParticleEmitters.begin() + i);
				break;
			}
		}
	}

	MeshEmitter* pMeshEmitter = dynamic_cast<MeshEmitter*>(pGameObject);
	if (pMeshEmitter != nullptr)
	{
		for (int i = 0; i < m_MeshEmitters.size(); i++)
		{
			if (m_MeshEmitters[i] == pGameObject)
			{
				m_MeshEmitters.erase(m_MeshEmitters.begin() + i);
				break;
			}
		}
	}

	const std::string& name = pGameObject->GetName();
	if (name != "")
	{
		m_NamedObjects[name] = nullptr;
	}


	pGameObject->OnRemovedFromScene(this);
}

void Scene::ExtendScene() noexcept
{
	m_Extending = true;
	m_Extended = !m_Extended;
}

void Scene::RegisterPickableGameObject(GameObject* pickableObject) noexcept
{
	m_PickableGameObjects.push_back(pickableObject);
}

void Scene::OnSceneExtensionComplete() noexcept
{
}

void Scene::OnUpdate(float dtS) noexcept
{
	for (int i = 0; i < m_UpdateAbles.size(); i++)
	{
		m_UpdateAbles[i]->Update(*m_pCamera, dtS);
	}

	if (m_Extending)
	{
		float extensionOffset = 20.0f * dtS * ((m_Extended * 2) - 1);

		m_Extension += extensionOffset;

		if (m_Extension > 10.0f)
		{
			m_Extending = false;
			m_Extension = 10.0f;
			extensionOffset = 0.0f;
			OnSceneExtensionComplete();
		}
		else if (m_Extension < 0.0f)
		{
			m_Extending = false;
			m_Extension = 0.0f;
			extensionOffset = 0.0f;
			OnSceneExtensionComplete();
		}

		if (m_pCamera->GetLookAt().y > 0.0f)
		{
			float heightFactor = m_pCamera->GetLookAt().y / 2.0f;
			float lookAtBoundsOffset = m_Extension * heightFactor;
			m_pCamera->SetMinXZMaxXZLookAt(lookAtBoundsOffset + 1.0f, 1.0f,
				lookAtBoundsOffset + 11.0f, 40.0f);
			m_pCamera->MoveWorldCoords(glm::vec3(extensionOffset * heightFactor, 0.0f, 0.0f), true);
		}
	}

	//Sort particleemitters so that the one furthest away gets rendered first
	//std::sort(m_ParticleEmitters.begin(), m_ParticleEmitters.end(), ParticleEmitter::DistGreater);
}
