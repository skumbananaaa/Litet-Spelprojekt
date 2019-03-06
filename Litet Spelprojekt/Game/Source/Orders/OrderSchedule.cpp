#include "..\..\Include\Orders\OrderSchedule.h"
#include "..\..\Include\Orders\OrderSleep.h"
#include <Graphics/Scene.h>
#include <System/Random.h>

std::vector<GameObject*> OrderSchedule::s_Beds;

void OrderSchedule::Init(Scene* pScene)
{
	GameObject* pGameObject = nullptr;
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Bunk Bed" + std::to_string(i++))) != nullptr; i++)
	{
		s_Beds.push_back(pGameObject);
	}
}

IOrder* OrderSchedule::GetIdleOrder()
{
	GameObject* pGameObject = s_Beds[Random::GenerateInt(0, s_Beds.size() - 1)];
	return new OrderSleep(pGameObject->GetTile(), pGameObject);
}
