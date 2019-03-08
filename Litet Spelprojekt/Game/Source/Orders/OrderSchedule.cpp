#include "..\..\Include\Orders\OrderSchedule.h"
#include "..\..\Include\Orders\OrderSleep.h"
#include "..\..\Include\Orders\OrderToilet.h"
#include "..\..\Include\Orders\OrderWork.h"
#include <Graphics/Scene.h>
#include <System/Random.h>

std::vector<GameObject*> OrderSchedule::s_Beds;
std::vector<GameObject*> OrderSchedule::s_Toilets;
std::vector<GameObject*> OrderSchedule::s_Instruments;

void OrderSchedule::Init(Scene* pScene)
{
	GameObject* pGameObject = nullptr;
	//Find beds
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Bunk Bed" + std::to_string(i))) != nullptr; i++)
	{
		s_Beds.push_back(pGameObject);
	}
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Single Bed" + std::to_string(i))) != nullptr; i++)
	{
		s_Beds.push_back(pGameObject);
	}

	//Find toilets
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Toilet" + std::to_string(i))) != nullptr; i++)
	{
		s_Toilets.push_back(pGameObject);
	}

	//Find instruments
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Instrument 1" + std::to_string(i))) != nullptr; i++)
	{
		s_Instruments.push_back(pGameObject);
	}
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Instrument 2" + std::to_string(i))) != nullptr; i++)
	{
		s_Instruments.push_back(pGameObject);
	}
}

IOrder* OrderSchedule::GetIdleOrder()
{
	uint32 order = Random::GenerateInt(0, 2);
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
	else if (order == 2 && !s_Instruments.empty())
	{
		pGameObject = s_Instruments[Random::GenerateInt(0, s_Instruments.size() - 1)];
		return new OrderWork(pGameObject->GetTile(), pGameObject);
	}

	return nullptr;
}
