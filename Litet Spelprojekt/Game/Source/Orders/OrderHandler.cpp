#include "../../Include/Orders/OrderHandler.h"
#include "../../Include/Game.h"
#include "../../Include/Orders/OrderWalk.h"

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

	for (IOrder* order : m_OrdersToAbort)
	{
		DeleteSafe(order);
	}
	m_OrdersToAbort.clear();

	std::cout << "Orders cleared" << std::endl;
}

void OrderHandler::GiveOrder(IOrder* order, Crewmember* crewMember) noexcept
{
	int index = -1;
	if (!order->IsIdleOrder())
	{
		for (int32 i = m_OrderQueue.size() - 1; i >= 0; i--)
		{
			if (m_OrderQueue[i]->IsIdleOrder())
			{
				m_OrdersToAbort.push_back(m_OrderQueue[i]);
				m_OrderQueue.erase(m_OrderQueue.begin() + i);
			}
		}
	}

	if (!CanExecuteOrder(order, crewMember))
	{
		m_OrdersToAbort.push_back(order);
		return;
	}
	else if (!order->AllowsMultipleOrders())
	{
		for (int i = 0; i < m_OrderQueue.size(); i++)
		{
			if (m_OrderQueue[i]->GetName().compare(order->GetName()) == 0)
			{
				m_OrdersToAbort.push_back(m_OrderQueue[i]);
				m_OrderQueue[i] = order;
				index = i;
				break;
			}
		}
		if (index == -1)
		{
			index = m_OrderQueue.size();
			m_OrderQueue.push_back(order);
		}
	}
	else
	{
		index = m_OrderQueue.size();
		m_OrderQueue.push_back(order);
	}
	order->m_pCrewMember = crewMember;

	if (index == 0)
	{
		StartOrder();
	}
}

void OrderHandler::Update(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	//Abort all orders that wants to be deleted
	for (int i = m_OrdersToAbort.size() - 1; i >= 0; i--)
	{
		if (m_OrdersToAbort[i]->ReadyToAbort())
		{
			m_OrdersToAbort[i]->AbortOrder(pScene, pWorld, pCrewMembers);
			std::cout << "[" << m_OrdersToAbort[i]->GetName() << "] Order Aborted" << std::endl;
			DeleteSafe(m_OrdersToAbort[i]);
			m_OrdersToAbort.erase(m_OrdersToAbort.begin() + i);
		}
	}

	if (!m_OrderQueue.empty())
	{
		//Check if current order is still executable
		if (!CanExecuteOrder(m_OrderQueue[0], m_OrderQueue[0]->GetCrewMember()))
		{
			if (!StartOrder())
			{
				m_pOrderListener->OnAllOrdersFinished();
			}
			return;
		}

		//Update Current Order
		if (m_OrderQueue[0]->UpdateOrder(pScene, pWorld, pCrewMembers, dtS))
		{
			m_OrderQueue[0]->EndOrder(pScene, pWorld, pCrewMembers);
			std::cout << "[" << m_OrderQueue[0]->GetName() << "] Order Ended" << std::endl;
			//m_OrderQueue[0]->m_pCrewMember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_IDLE);
			DeleteSafe(m_OrderQueue[0]);
			m_OrderQueue.erase(m_OrderQueue.begin());

			if (!StartOrder())
			{
				m_pOrderListener->OnAllOrdersFinished();
			}
		}
	}
}

bool OrderHandler::StartOrder()
{
	if (m_OrderQueue.empty())
	{
		return false;
	}
	else if (!CanExecuteOrder(m_OrderQueue[0], m_OrderQueue[0]->GetCrewMember()))
	{
		m_OrdersToAbort.push_back(m_OrderQueue[0]);
		m_OrderQueue.erase(m_OrderQueue.begin());
		return StartOrder();
	}

	SceneGame* pSceneGame = Game::GetGame()->m_pSceneGame;
	m_OrderQueue[0]->StartOrder(pSceneGame, pSceneGame->GetWorld(), pSceneGame->GetCrew());
	std::cout << "[" << m_OrderQueue[0]->GetName() << "] Order Started" << std::endl;

	m_pOrderListener->OnOrderStarted(m_OrderQueue[0]->IsIdleOrder());
	return true;
}

bool OrderHandler::CanExecuteOrder(IOrder* order, Crewmember* crewMember) noexcept
{
	if (!crewMember->IsAbleToWalk())
	{
		OrderWalk* pOrderWalk = dynamic_cast<OrderWalk*>(order);
		if (pOrderWalk)
		{
			return false;
		}
	}
	else if (!crewMember->IsAbleToWork())
	{
		if (!order->CanExecuteIfHurt())
		{
			return false;
		}
	}
	return true;
}
