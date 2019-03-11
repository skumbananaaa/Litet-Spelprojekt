#include <Graphics/Scene.h>
#include <System/Random.h>
#include "..\..\Include\Orders\OrderSchedule.h"
#include "..\..\Include\Orders\OrderSleep.h"
#include "..\..\Include\Orders\OrderToilet.h"
#include "..\..\Include\Orders\OrderWork.h"
#include "..\..\Include\Orders\OrderEat.h"
#include "..\..\Include\Orders\OrderCook.h"

std::vector<GameObject*> OrderSchedule::s_Beds;
std::vector<GameObject*> OrderSchedule::s_Toilets;
std::vector<GameObject*> OrderSchedule::s_Instruments;
std::vector<GameObject*> OrderSchedule::s_Chairs;
std::vector<GameObject*> OrderSchedule::s_Ovens;

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

	//Find chairs
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Chair" + std::to_string(i))) != nullptr; i++)
	{
		s_Chairs.push_back(pGameObject);
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

	//Find ovens
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Oven" + std::to_string(i))) != nullptr; i++)
	{
		s_Ovens.push_back(pGameObject);
	}
}

void OrderSchedule::Release()
{
	s_Beds.clear();
	s_Toilets.clear();
	s_Instruments.clear();
	s_Chairs.clear();
	s_Ovens.clear();
}

IOrder* OrderSchedule::GetIdleOrder()
{
	uint32 order = Random::GenerateInt(0, 4);
	GameObject* pGameObject = nullptr;
	if (order == 0 && !s_Beds.empty())
	{
		pGameObject = s_Beds[Random::GenerateInt(0, s_Beds.size() - 1)];
		return new OrderSleep(pGameObject->GetTile(), pGameObject);
	}
	else if (order == 1 && !s_Toilets.empty())
	{
		pGameObject = s_Toilets[Random::GenerateInt(0, s_Toilets.size() - 1)];
		return new OrderToilet(pGameObject->GetTile(), pGameObject);
	}
	else if (order == 2 && !s_Instruments.empty())
	{
		pGameObject = s_Instruments[Random::GenerateInt(0, s_Instruments.size() - 1)];
		return new OrderWork(pGameObject->GetTile(), pGameObject);
	}
	else if (order == 3 && !s_Chairs.empty())
	{
		pGameObject = s_Chairs[Random::GenerateInt(0, s_Chairs.size() - 1)];
		return new OrderEat(pGameObject->GetTile(), pGameObject);
	}
	else if (order == 4 && !s_Ovens.empty())
	{
		pGameObject = s_Ovens[Random::GenerateInt(0, s_Ovens.size() - 1)];
		return new OrderCook(pGameObject->GetTile(), pGameObject);
	}

	return nullptr;
}
