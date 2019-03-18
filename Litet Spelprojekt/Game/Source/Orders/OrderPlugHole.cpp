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

OrderPlugHole::OrderPlugHole(const glm::ivec3& roomTile, const glm::ivec3& holeTile, bool hasGearEquipped)
	: OrderWalk(startTarget(roomTile, holeTile, hasGearEquipped)),
	m_EquippingGearTimer(EQUIPTIME),
	m_PluggingTimer(PLUGTIME)
{
	m_RoomTile = roomTile;
	m_HoleTile = holeTile;
	m_PluggingHole = false;
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
				glm::ivec3 tile = GetCrewMember()->GetTile();
				if (pWorld->GetLevel(tile.y).GetLevelData()[tile.x][tile.z].WaterInlet)
				{
					if (pWorld->GetLevel(tile.y).GetLevelData()[tile.x][tile.z].WaterInlet)
					{
						Logger::LogEvent(GetCrewMember()->GetName() + " pluggade igen hålet!", true);
					}
					else
					{
						Logger::LogEvent(GetCrewMember()->GetName() + " kontrollerade det pluggningen!", true);
					}
					pWorld->GetLevel(tile.y).GetLevelData()[tile.x][tile.z].WaterInlet = false;
					if (!pWorld->GetRoom(GetCrewMember()->GetRoom()).IsPumping())
					{
						pCrewmember->GiveOrder(new OrderPumpWater(GetCrewMember()->GetRoom(), pWorld->FindClosestRoomInInterval(MACHINE_ROOM_INTERVAL_START, MACHINE_ROOM_INTERVAL_END, tile)));
					}
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
				pCrewmember->GiveOrder(new OrderPlugHole(m_RoomTile, m_HoleTile, pCrewmember->HasGearEquipped()));
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
