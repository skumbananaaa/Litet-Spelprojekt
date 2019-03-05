#include "../../Include/Orders/OrderHandler.h"
#include "../../Include/Game.h"

OrderHandler::OrderHandler()
{
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

	order->m_pCrewMember = crewMember;
	m_OrderQueue.push_back(order);

	if (m_OrderQueue.size() == 1)
	{
		StartOrder();
	}
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

		if (!m_OrderQueue.empty())
		{
			StartOrder();
		}
	}
}

void OrderHandler::StartOrder()
{
	SceneGame* pSceneGame = Game::GetGame()->m_pSceneGame;
	m_OrderQueue[0]->StartOrder(pSceneGame, pSceneGame->GetWorld(), pSceneGame->GetCrew());
	std::cout << "[" << m_OrderQueue[0]->GetName() << "] Order Started" << std::endl;
}
