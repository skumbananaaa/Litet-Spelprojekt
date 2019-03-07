#include "..\..\Include\Orders\OrderSchedule.h"
#include "..\..\Include\Orders\OrderSleep.h"
#include "..\..\Include\Orders\OrderToilet.h"
#include <Graphics/Scene.h>
#include <System/Random.h>

std::vector<GameObject*> OrderSchedule::s_Beds;
std::vector<GameObject*> OrderSchedule::s_Toilets;

void OrderSchedule::Init(Scene* pScene)
{
	GameObject* pGameObject = nullptr;
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Bunk Bed" + std::to_string(i))) != nullptr; i++)
	{
		s_Beds.push_back(pGameObject);
	}

	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Toilet" + std::to_string(i))) != nullptr; i++)
	{
		s_Toilets.push_back(pGameObject);
	}
}

IOrder* OrderSchedule::GetIdleOrder()
{
	uint32 order = Random::GenerateInt(0, 1);
	GameObject* pGameObject = nullptr;
	if (order == 0)
	{
		pGameObject = s_Beds[Random::GenerateInt(0, s_Beds.size() - 1)];
		return new OrderSleep(pGameObject->GetTile(), pGameObject);
	}
	else if (order == 1)
	{
		pGameObject = s_Toilets[Random::GenerateInt(0, s_Toilets.size() - 1)];
		return new OrderToilet(pGameObject->GetTile(), pGameObject);
	}

	return nullptr;
}
