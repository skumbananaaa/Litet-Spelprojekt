#include "../../Include/Orders/OrderPlugHole.h"
#include "../../Include/Crewmember.h"
#include "../../Include/Orders/OrderPumpWater.h"
#include <World/World.h>
glm::ivec3 startTarget(const glm::ivec3& roomTile, const glm::ivec3& holeTile, bool hasGearEquipped)
{
	if (hasGearEquipped)
	{
		return holeTile;
	}

	return roomTile;
}

OrderPlugHole::OrderPlugHole(OrderPlugHole* other) : OrderWalk(other),
	m_EquippingGearTimer(EQUIPTIME),
	m_PluggingTimer(PLUGTIME)
{
	m_RoomTile = other->m_RoomTile;
	m_HoleTile = other->m_HoleTile;
	m_PluggingHole = false;
	m_HolePlugged = false;
}

OrderPlugHole::OrderPlugHole(const glm::ivec3& roomTile, const glm::ivec3& holeTile, bool hasGearEquipped)
	: OrderWalk(startTarget(roomTile, holeTile, hasGearEquipped)),
	m_EquippingGearTimer(EQUIPTIME),
	m_PluggingTimer(PLUGTIME)
{
	m_RoomTile = roomTile;
	m_HoleTile = holeTile;
	m_PluggingHole = false;
	m_HolePlugged = false;
}

OrderPlugHole::~OrderPlugHole()
{
}

void OrderPlugHole::OnStarted(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::OnStarted(pScene, pWorld, pCrewMembers);
}

void OrderPlugHole::OnEnded(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);

	GetCrewMember()->ReportPosition();
}

bool OrderPlugHole::OnUpdate(Scene * pScene, World * pWorld, Crew * pCrewMembers, float dtS) noexcept
{
	Crewmember* pCrewmember = GetCrewMember();
	bool res = OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS);
	if (pCrewmember->HasGearEquipped())
	{
		// Run to hole
		if (res)
		{
			res = false;
			// if reached, plug the hole!
			if (!m_PluggingHole)
			{
				m_PluggingHole = true;
				pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_WORK);
			}

			m_PluggingTimer -= dtS;

			if (m_PluggingTimer <= 0.0001)
			{
				m_HolePlugged = true;
				glm::ivec3 tile = GetCrewMember()->GetTile();

				if (pWorld->GetLevel(tile.y).GetLevelData()[tile.x][tile.z].WaterInlet)
				{
					pWorld->GetLevel(tile.y).GetLevelData()[tile.x][tile.z].WaterInlet = false;
					GiveOrder(new OrderPumpWater(GetCrewMember()->GetRoom(), glm::ivec3(3, 3, 3)));
				}
				res = true;
			}
		}
	}
	else
	{
		// Run to equipment room
		if (res)
		{
			res = false;
			//if reached equipment, equip it!
			if (m_EquippingGearTimer >= EQUIPTIME)
			{
				pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_OPENDOOR);
			}
			m_EquippingGearTimer -= dtS;

			if (m_EquippingGearTimer <= 0.0000001)
			{
				pCrewmember->SetGearIsEquipped(true);
				GiveOrder(new OrderPlugHole(m_RoomTile, m_HoleTile, pCrewmember->HasGearEquipped()));
				res = true;
			}

		}
	}
	return res;
}

bool OrderPlugHole::CanBeStackedWithSameType() noexcept
{
	return false;
}

std::string OrderPlugHole::GetName() noexcept
{
	return "Plug hole";
}

bool OrderPlugHole::IsIdleOrder() noexcept
{
	return false;
}

IOrder * OrderPlugHole::Clone() noexcept
{
	return new OrderPlugHole(this);
}

void OrderPlugHole::BeginReplay(SceneGame * pScene, void * userData) noexcept
{
	OrderWalk::BeginReplay(pScene, userData);
}
