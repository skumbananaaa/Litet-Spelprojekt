#include <EnginePch.h>
#include <Graphics\Scene.h>

Scene::Scene() noexcept
{
	m_pCamera = nullptr;
}

Scene::~Scene()
{
	if (m_pCamera != nullptr)
	{
		delete m_pCamera;
	}

	for (unsigned int i = 0; i < m_GameObjects.size(); i++)
	{
		delete m_GameObjects[i];
		m_GameObjects[i] = nullptr;
	}
}

void Scene::SetCamera(Camera* pCamera) noexcept
{
	m_pCamera = pCamera;
}

void Scene::AddGameObject(GameObject* pGameObject) noexcept
{
	m_GameObjects.push_back(pGameObject);
}

void Scene::RemoveGameObject(unsigned int index) noexcept
{
	m_GameObjects.erase(m_GameObjects.begin() + index);
}

void Scene::OnUpdate(float dtS) noexcept
{
	m_pCamera->Update();
	
	for (unsigned int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->UpdateTransform();
	}
}
