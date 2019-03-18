#include "../../Include/Orders/OrderHandler.h"
#include "../../Include/Game.h"
#include "../../Include/Orders/OrderWalk.h"
#include "../../Include/Orders/OrderWalkMedicBay.h"
#include "../../Include/ReplayHandler.h"

#define NOT_FOUND -1
std::vector<IOrder*> OrderHandler::s_OrderCopies[NUM_CREW];

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

	if (!m_OrderQueue.empty() && dynamic_cast<OrderWalkMedicBay*>(m_OrderQueue[0]))
	{
		DeleteSafe(order);
		return;
	}

	order->m_pCrewMember = m_pCrewmember;

	if (ReplayHandler::IsReplaying() || !IsCrewMemberAbleToExecuteOrder(order))
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

void OrderHandler::Reset() noexcept
{
	for (int i = 0; i < NUM_CREW; i++)
	{
		for (int j = 0; j < s_OrderCopies[i].size(); j++)
		{
			DeleteSafe(s_OrderCopies[i][j]);
		}
		s_OrderCopies[i].clear();
	}
}

void OrderHandler::ForceOrder(SceneGame* pScene, void* userData, IOrder* order) noexcept
{
	for (int i = m_OrderQueue.size() - 1; i >= 0; i--)
	{
		m_OrderQueue[i]->m_IsAborted = true;
		m_OrdersToDelete.push_back(m_OrderQueue[i]);
	}
	m_OrderQueue.clear();
	IOrder* pOrderClone = order->Clone();
	pOrderClone->InitClone(pScene, userData);
	m_OrderQueue.push_back(pOrderClone);
	StartNextExecutableOrder();
}

void OrderHandler::ForceOrderInbreed(IOrder* order) noexcept
{
	for (int i = m_OrderQueue.size() - 1; i >= 0; i--)
	{
		m_OrderQueue[i]->m_IsAborted = true;
		m_OrdersToDelete.push_back(m_OrderQueue[i]);
	}
	m_OrderQueue.clear();
	m_OrderQueue.push_back(order);
	StartNextExecutableOrder();
}

bool OrderHandler::StartNextExecutableOrder()
{
	if (m_OrderQueue.empty())
	{
		return false;
	}
	else if (!IsCrewMemberAbleToExecuteOrder(m_OrderQueue[0]))
	{
		m_OrderQueue[0]->m_IsAborted = true;
		m_OrdersToDelete.push_back(m_OrderQueue[0]);
		m_OrderQueue.erase(m_OrderQueue.begin());
		return StartNextExecutableOrder();
	}

	SceneGame* pSceneGame = Game::GetGame()->m_pSceneGame;
	IOrder* pOrder = m_OrderQueue[0];
	pOrder->OnStarted(pSceneGame, pSceneGame->GetWorld(), pSceneGame->GetCrew());
	//std::cout << "[" << pOrder->GetName() << "][" << m_pCrewmember->GetName() << "] Order Started" << std::endl;

	if (!ReplayHandler::IsReplaying())
	{
		if (!pOrder->m_IsInbred)
		{
			IOrder* pOrderClone = pOrder->Clone();
			pOrderClone->RegisterReplayEvent(nullptr);
			s_OrderCopies[m_pCrewmember->GetShipNumber()].push_back(pOrderClone);
		}
	}

	m_pCrewmember->OnOrderStarted(pOrder->IsIdleOrder());
	pOrder->OnStarted(pSceneGame, pSceneGame->GetWorld(), pSceneGame->GetCrew());
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
			m_OrderQueue[i]->m_IsAborted = true;
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
			m_OrderQueue[i]->m_IsAborted = true;
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
		m_OrderQueue[i]->m_IsAborted = true;
		m_OrdersToDelete.push_back(m_OrderQueue[i]);
	}
	m_OrderQueue.clear();
}

void OrderHandler::DeleteRemovedOrders(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	for (int i = m_OrdersToDelete.size() - 1; i >= 0; i--)
	{
		IOrder* pOrder = m_OrdersToDelete[i];
		if (pOrder->ReadyToAbort())
		{
			if(!pOrder->m_IsAborted)
			{
				pOrder->OnEnded(pScene, pWorld, pCrewMembers);
			}
			else
			{
				pOrder->OnAborted(pScene, pWorld, pCrewMembers);
			}
			
			//std::cout << "[" << pOrder->GetName() << "][" << m_pCrewmember->GetName() << "] Order Ended" << std::endl;
			DeleteSafe(pOrder);
			m_OrdersToDelete.erase(m_OrdersToDelete.begin() + i);
		}
	}
}