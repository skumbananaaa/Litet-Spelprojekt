#include "..\..\Include\Orders\IOrder.h"
#include "../../Include/Crewmember.h"
#include "../../Include/Scenes/SceneGame.h"
#include "../../Include/Orders/OrderHandler.h"
#include "../../Include/ReplayHandler.h"

IOrder::IOrder(IOrder * other) : m_IsAborted(false)
{
	m_ShipId = other->m_pCrewMember->GetShipNumber();
	m_IsInbred = false;
}


void IOrder::InitClone(SceneGame * pScene, void* userData) noexcept
{
	m_pCrewMember = pScene->GetCrew()->GetMember(m_ShipId);
}

void IOrder::BeginReplay(SceneGame* pScene, void* userData) noexcept
{
	m_pCrewMember = pScene->GetCrew()->GetMember(m_ShipId);

	if (userData == nullptr)
	{
		m_pCrewMember->m_OrderHandler.ForceOrder(pScene, userData, this);
	}
	else
	{
		const glm::vec3* pPosition = reinterpret_cast<glm::vec3*>(userData);
		std::cout << "Position Set From Order: " << glm::to_string(*pPosition) << std::endl;
		m_pCrewMember->SetPosition(*pPosition);
	}
}

void IOrder::GiveOrderInbred(IOrder* order) noexcept
{
	if (!ReplayHandler::IsReplaying())
	{
		order->m_IsInbred = true;
		m_pCrewMember->GiveOrder(order);
	}
	else
	{
		m_pCrewMember->m_OrderHandler.GiveFilteredOrder(order);
	}
}
