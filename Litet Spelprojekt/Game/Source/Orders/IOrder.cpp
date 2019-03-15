#include "..\..\Include\Orders\IOrder.h"
#include "../../Include/Crewmember.h"
#include "../../Include/Scenes/SceneGame.h"
#include "../../Include/Orders/OrderHandler.h"

IOrder::IOrder(IOrder * other)
{
	m_ShipId = other->m_pCrewMember->GetShipNumber();
	m_IsInbred = false;
}


void IOrder::BeginReplay(SceneGame* pScene, void* userData) noexcept
{
	m_pCrewMember = pScene->GetCrew()->GetMember(m_ShipId);
	//m_pCrewMember->GiveOrder(this);
	m_pCrewMember->m_OrderHandler.ForceOrder(this);
}

void IOrder::GiveOrder(IOrder* order) noexcept
{
	order->m_IsInbred = true;
	m_pCrewMember->GiveOrder(order);
}
