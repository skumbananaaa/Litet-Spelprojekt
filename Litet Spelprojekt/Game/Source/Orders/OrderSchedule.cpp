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
std::vector<GameObject*> OrderSchedule::s_Extinguishers;

void OrderSchedule::Init(Scene* pScene)
{
	GameObject* pGameObject = nullptr;
	//Find beds
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Bunkbed" + std::to_string(i))) != nullptr; i++)
	{
		s_Beds.push_back(pGameObject);
	}
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Singlebed" + std::to_string(i))) != nullptr; i++)
	{
		s_Beds.push_back(pGameObject);
	}

	//Find toilets
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Toalett" + std::to_string(i))) != nullptr; i++)
	{
		s_Toilets.push_back(pGameObject);
	}

	//Find chairs
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Stol" + std::to_string(i))) != nullptr; i++)
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

	//Find extinguishers
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Extinguisher" + std::to_string(i))) != nullptr; i++)
	{
		s_Extinguishers.push_back(pGameObject);
	}

	//Find ovens
	for (uint32 i = 0; (pGameObject = pScene->GetGameObject("Ugn" + std::to_string(i))) != nullptr; i++)
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
	IOrder* pOrder = nullptr;
	for (uint32 i = 0; i < 5 && pOrder == nullptr; i++)
	{
		uint32 order = Random::GenerateInt(0, 100);
		if (order >= 60)
		{
			pOrder = GetOrderSleep();
		}
		else if (order >= 20)
		{
			pOrder = GetOrderWork();
		}
		else if (order >= 5)
		{
			pOrder = GetOrderCook();
		}
		else if (order >= 0)
		{
			pOrder = GetOrderToilet();
		}
	}

	return pOrder;
}

IOrder* OrderSchedule::GetOrderEat()
{
	if (!s_Chairs.empty())
	{
		GameObject* pGameObject = s_Chairs[Random::GenerateInt(0, (int32)s_Chairs.size() - 1)];
		return new OrderEat(pGameObject->GetTile(), pGameObject);
	}

	return nullptr;
}

IOrder* OrderSchedule::GetOrderSleep()
{
	if (!s_Beds.empty())
	{
		GameObject* pGameObject = s_Beds[Random::GenerateInt(0, (int32)s_Beds.size() - 1)];
		return new OrderSleep(pGameObject->GetTile(), pGameObject);
	}

	return nullptr;
}

IOrder* OrderSchedule::GetOrderCook()
{
	if (!s_Ovens.empty())
	{
		GameObject* pGameObject = s_Ovens[Random::GenerateInt(0, (int32)s_Ovens.size() - 1)];
		return new OrderCook(pGameObject->GetTile(), pGameObject);
	}

	return nullptr;
}

IOrder* OrderSchedule::GetOrderWork()
{
	if (!s_Instruments.empty())
	{
		GameObject* pGameObject = s_Instruments[Random::GenerateInt(0, (int32)s_Instruments.size() - 1)];
		return new OrderWork(pGameObject->GetTile(), pGameObject);
	}

	return nullptr;
}

IOrder* OrderSchedule::GetOrderToilet()
{

	if (!s_Toilets.empty())
	{
		GameObject* pGameObject = s_Toilets[Random::GenerateInt(0, (int32)s_Toilets.size() - 1)];
		return new OrderToilet(pGameObject->GetTile(), pGameObject);
	}

	return nullptr;
}
