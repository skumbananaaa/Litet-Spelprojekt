#include "..\..\Include\Orders\OrderExtinguishFire.h"
#include "../../Include/Crewmember.h"
#include <World/World.h>

OrderExtinguishFire::OrderExtinguishFire(const glm::ivec3& roomTile, const glm::ivec3& burningTile, uint32 roomBurningId, bool hasGearEquipped) :
	OrderWalk(hasGearEquipped ? burningTile : roomTile),
	m_EquippingGearTimer(0.0f)
{
	m_RoomBurningId = roomBurningId;
	m_RoomTile = roomTile;
	m_BurningTile = burningTile;
	std::cout << "Extinguish Fire Order Room Tile: " << glm::to_string(roomTile) << std::endl;
	std::cout << "Extinguish Fire Order Burning Tile: " << glm::to_string(burningTile) << std::endl;
}

OrderExtinguishFire::~OrderExtinguishFire()
{
}

void OrderExtinguishFire::StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::StartOrder(pScene, pWorld, pCrewMembers);
}

bool OrderExtinguishFire::UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	Crewmember* pCrewMember = GetCrewMember();

	if (!pCrewMember->HasGearEquipped())
	{
		//Run To Room
		if (OrderWalk::UpdateOrder(pScene, pWorld, pCrewMembers, dtS))
		{
			if (m_EquippingGearTimer <= 0.00001f)
			{
				pCrewMember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_OPENDOOR);
			}

			//Equip Gear
			if ((m_EquippingGearTimer += dtS) > TIME_TO_EQUIP_GEAR)
			{
				m_EquippingGearTimer = 0.0f;
				pCrewMember->SetGearIsEquipped(true);
				//pCrewMember->GiveOrder(new OrderExtinguishFire(m_RoomTile, m_BurningTile, m_RoomBurningId, true));
				OrderWalk::RestartOrder(pScene, pWorld, pCrewMembers, m_BurningTile);
				return false;
			}
		}
	}
	else
	{
		//Run To Room That is Burning
		if (OrderWalk::UpdateOrder(pScene, pWorld, pCrewMembers, dtS))
		{
			TileData& burningTile = pWorld->GetLevel(m_BurningTile.y).GetLevelData()[m_BurningTile.x][m_BurningTile.z];

			//Extinguish Fire
			if (burningTile.Temp > burningTile.BurnsAt)
			{
				burningTile.Temp -= FIRE_EXTINGUISH_TEMP_REDUCTION * dtS;
				return false;
			}
			else
			{
				//pCrewMember->GiveOrder(new OrderWalk(m_RoomTile));
				return true;
			}
		}
	}

	return false;

	//Run out if dying or our fire out of control
}

void OrderExtinguishFire::EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::EndOrder(pScene, pWorld, pCrewMembers);
}

void OrderExtinguishFire::AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::AbortOrder(pScene, pWorld, pCrewMembers);
}

bool OrderExtinguishFire::AllowsMultipleOrders() noexcept
{
	return false;
}

std::string OrderExtinguishFire::GetName() noexcept
{
	return "Extinguish Fire";
}

bool OrderExtinguishFire::IsIdleOrder() noexcept
{
	return false;
}

bool OrderExtinguishFire::CanExecuteIfHurt() noexcept
{
	return false;
}
