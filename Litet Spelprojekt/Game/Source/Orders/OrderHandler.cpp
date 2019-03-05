#include "../../Include/Orders/OrderHandler.h"
#include "../../Include/Game.h"

OrderHandler::OrderHandler(IOrderListener* pOrderListener)
{
	m_pOrderListener = pOrderListener;
}

OrderHandler::~OrderHandler()
{
	for (IOrder* order : m_OrderQueue)
	{
		DeleteSafe(order);
	}
	m_OrderQueue.clear();
	std::cout << "Orders cleared" << std::endl;
}

void OrderHandler::GiveOrder(IOrder* order, Crewmember* crewMember) noexcept
{
	if (!order->AllowsMultipleOrders())
	{
		for (IOrder* object : m_OrderQueue)
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
	m_OrderQueue.push_back(order);
	std::cout << "[" << order->GetName() << "] Order Started" << std::endl;
}

void OrderHandler::Update(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	for (int i = m_OrderQueue.size() - 1; i >= 0; i--)
	{
		if (m_OrderQueue[i]->m_Abort)
		{
			if (m_OrderQueue[i]->ReadyToAbort())
			{
				m_OrderQueue[i]->AbortOrder(pScene, pWorld, pCrewMembers);
				std::cout << "[" << m_OrderQueue[i]->GetName() << "] Order Aborted" << std::endl;
				DeleteSafe(m_OrderQueue[i]);
				m_OrderQueue.erase(m_OrderQueue.begin() + i);
			}
		}
	}

	if (!m_OrderQueue.empty() && m_OrderQueue[0]->UpdateOrder(pScene, pWorld, pCrewMembers, dtS))
	{
		m_OrderQueue[0]->EndOrder(pScene, pWorld, pCrewMembers);
		std::cout << "[" << m_OrderQueue[0]->GetName() << "] Order Ended" << std::endl;
		DeleteSafe(m_OrderQueue[0]);
		m_OrderQueue.erase(m_OrderQueue.begin());
	}
}