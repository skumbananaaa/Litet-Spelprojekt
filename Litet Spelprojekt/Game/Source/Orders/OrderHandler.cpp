#include "../../Include/Orders/OrderHandler.h"
#include "../../Include/Game.h"

OrderHandler::OrderHandler()
{
}

OrderHandler::~OrderHandler()
{
	for (IOrder* order : s_OrderQueue)
	{
		DeleteSafe(order);
	}
	s_OrderQueue.clear();
	std::cout << "Orders cleared" << std::endl;
}

void OrderHandler::GiveOrder(IOrder* order, Crewmember* crewMember) noexcept
{
	if (!order->AllowsMultipleOrders())
	{
		for (IOrder* object : s_OrderQueue)
		{
			if (object->GetName().compare(order->GetName()) == 0)
			{
				object->m_Abort = true;
				break;
			}
		}
	}

	SceneGame* pSceneGame = Game::GetGame()->m_pSceneGame;
	order->m_pCrewMember = crewMember;
	order->StartOrder(pSceneGame, pSceneGame->GetWorld(), pSceneGame->GetCrew());
	s_OrderQueue.push_back(order);
	std::cout << "[" << order->GetName() << "] Order Started" << std::endl;
}

void OrderHandler::Update(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	for (int i = s_OrderQueue.size() - 1; i >= 0; i--)
	{
		if (s_OrderQueue[i]->m_Abort)
		{
			if (s_OrderQueue[i]->ReadyToAbort())
			{
				s_OrderQueue[i]->AbortOrder(pScene, pWorld, pCrewMembers);
				std::cout << "[" << s_OrderQueue[i]->GetName() << "] Order Aborted" << std::endl;
				DeleteSafe(s_OrderQueue[i]);
				s_OrderQueue.erase(s_OrderQueue.begin() + i);
			}
		}
	}

	if (!s_OrderQueue.empty() && s_OrderQueue[0]->UpdateOrder(pScene, pWorld, pCrewMembers, dtS))
	{
		s_OrderQueue[0]->EndOrder(pScene, pWorld, pCrewMembers);
		std::cout << "[" << s_OrderQueue[0]->GetName() << "] Order Ended" << std::endl;
		DeleteSafe(s_OrderQueue[0]);
		s_OrderQueue.erase(s_OrderQueue.begin());
	}
}