#include "../../Include/Orders/OrderHandler.h"
#include "../../Include/Game.h"
#include "../../Include/Orders/OrderWalk.h"

#define NOT_FOUND -1

OrderHandler::OrderHandler(Crewmember* pCrewMember)
{
	m_pCrewmember = pCrewMember;
}

OrderHandler::~OrderHandler()
{
	for (IOrder* order : m_OrderQueue)
	{
		DeleteSafe(order);
	}
	m_OrderQueue.clear();

	for (IOrder* order : m_OrdersToDelete)
	{
		DeleteSafe(order);
	}
	m_OrdersToDelete.clear();

	std::cout << "Orders cleared" << std::endl;
}

void OrderHandler::GiveOrder(IOrder* order) noexcept
{
	if (!order)
	{
		return;
	}

	if (!m_OrderQueue.empty() && m_OrderQueue[0]->HasPriority())
	{
		DeleteSafe(order);
		return;
	}

	order->m_pCrewMember = m_pCrewmember;

	if (!IsCrewMemberAbleToExecuteOrder(order))
	{
		DeleteSafe(order);
		return;
	}

	if (order->HasPriority())
	{
		RemoveAllOrders();
		m_OrderQueue.push_back(order);
	}
	else
	{
		if (!order->IsIdleOrder())
		{
			RemoveIdleOrders();
		}

		if (!order->CanBeStackedWithSameType())
		{
			int index = ReplaceOrderOfSameType(order);
			if (index > 0) //Order is put later in the stack
			{
				return;
			}
			else if (index == NOT_FOUND)
			{
				m_OrderQueue.push_back(order);
				if (m_OrderQueue.size() > 1)
				{
					return;
				}
			}
		}
		else
		{
			m_OrderQueue.push_back(order);
			if (m_OrderQueue.size() > 1)
			{
				return;
			}
		}
	}
	StartNextExecutableOrder();
}

void OrderHandler::Update(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	DeleteRemovedOrders(pScene, pWorld, pCrewMembers);
	
	if (!m_OrderQueue.empty())
	{
		IOrder* pCurrentOrder = m_OrderQueue[0];

		//Check if current order is still executable
		if (!IsCrewMemberAbleToExecuteOrder(pCurrentOrder))
		{
			if (!StartNextExecutableOrder())
			{
				m_pCrewmember->OnAllOrdersFinished();
			}
			return;
		}

		//Update Current Order
		if (pCurrentOrder->OnUpdate(pScene, pWorld, pCrewMembers, dtS))
		{
			std::vector<IOrder*>::iterator iterator = std::find(m_OrderQueue.begin(), m_OrderQueue.end(), pCurrentOrder);
			if (iterator != m_OrderQueue.end())
			{
				m_OrdersToDelete.push_back(pCurrentOrder);
				m_OrderQueue.erase(iterator);
				if (!StartNextExecutableOrder())
				{
					m_pCrewmember->OnAllOrdersFinished();
				}
			}
		}
	}
}

bool OrderHandler::StartNextExecutableOrder()
{
	if (m_OrderQueue.empty())
	{
		return false;
	}
	else if (!IsCrewMemberAbleToExecuteOrder(m_OrderQueue[0]))
	{
		m_OrdersToDelete.push_back(m_OrderQueue[0]);
		m_OrderQueue.erase(m_OrderQueue.begin());
		return StartNextExecutableOrder();
	}

	SceneGame* pSceneGame = Game::GetGame()->m_pSceneGame;
	IOrder* pOrder = m_OrderQueue[0];
	pOrder->OnStarted(pSceneGame, pSceneGame->GetWorld(), pSceneGame->GetCrew());
	std::cout << "[" << pOrder->GetName() << "] Order Started" << std::endl;

	m_pCrewmember->OnOrderStarted(pOrder->IsIdleOrder());
	return true;
}

bool OrderHandler::IsCrewMemberAbleToExecuteOrder(IOrder* order) noexcept
{
	if (!m_pCrewmember->IsAbleToWalk())
	{
		OrderWalk* pOrderWalk = dynamic_cast<OrderWalk*>(order);
		if (pOrderWalk)
		{
			return false;
		}
	}
	else if (!m_pCrewmember->IsAbleToWork())
	{
		if (!order->CanExecuteIfHurt())
		{
			return false;
		}
	}
	return true;
}

void OrderHandler::RemoveIdleOrders() noexcept
{
	for (int32 i = m_OrderQueue.size() - 1; i >= 0; i--)
	{
		if (m_OrderQueue[i]->IsIdleOrder())
		{
			m_OrdersToDelete.push_back(m_OrderQueue[i]);
			m_OrderQueue.erase(m_OrderQueue.begin() + i);
		}
	}
}

int OrderHandler::ReplaceOrderOfSameType(IOrder* order) noexcept
{
	for (int i = 0; i < m_OrderQueue.size(); i++)
	{
		if (m_OrderQueue[i]->GetName().compare(order->GetName()) == 0)
		{
			m_OrdersToDelete.push_back(m_OrderQueue[i]);
			m_OrderQueue[i] = order;
			return i;
		}
	}
	return NOT_FOUND;
}

void OrderHandler::RemoveAllOrders() noexcept
{
	for (int i = 0; i < m_OrderQueue.size(); i++)
	{
		m_OrdersToDelete.push_back(m_OrderQueue[i]);
	}
	m_OrderQueue.clear();
}

void OrderHandler::DeleteRemovedOrders(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	for (int i = m_OrdersToDelete.size() - 1; i >= 0; i--)
	{
		IOrder* order = m_OrdersToDelete[i];
		if (order->ReadyToAbort())
		{
			order->OnEnded(pScene, pWorld, pCrewMembers);
			std::cout << "[" << order->GetName() << "] Order Ended" << std::endl;
			DeleteSafe(order);
			m_OrdersToDelete.erase(m_OrdersToDelete.begin() + i);
		}
	}
}