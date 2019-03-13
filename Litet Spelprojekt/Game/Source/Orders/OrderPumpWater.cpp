#include "../../Include/Orders/OrderPumpWater.h"
#include <World/World.h>
#include "../../Include/Crewmember.h"
OrderPumpWater::OrderPumpWater(uint32 roomToPump, glm::ivec3 pumpControl)
	: OrderWalk(pumpControl),
	m_RoomToPump(roomToPump)
{
}

OrderPumpWater::~OrderPumpWater()
{
}

void OrderPumpWater::OnStarted(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::OnStarted(pScene, pWorld, pCrewMembers);
}

void OrderPumpWater::OnEnded(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);
}

bool OrderPumpWater::OnUpdate(Scene * pScene, World * pWorld, Crew * pCrewMembers, float dtS) noexcept
{
	bool clean = false;
	if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
	{
		if (!m_Pumping)
		{
			m_Pumping = true;
			GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_WORK);
		}

		Room* room = &pWorld->GetRoom(m_RoomToPump);

		uint32 size = room->GetTileData()->size();
		clean = true;
		for (uint32 i = 0; i < size; i++)
		{
			TileData* data = (*room->GetTileData())[i];

			if (data->WaterLevel > 0.0001)
			{
				clean = false;
				data->WaterLevel -= 0.01 * dtS;
			}
		}
	}
	return clean;
}

bool OrderPumpWater::CanBeStackedWithSameType() noexcept
{
	return false;
}

bool OrderPumpWater::HasPriority() noexcept
{
	return true;
}

std::string OrderPumpWater::GetName() noexcept
{
	return "Pump water from room " + std::to_string(m_RoomToPump);
}

bool OrderPumpWater::ReadyToAbort() noexcept
{
	return OrderWalk::ReadyToAbort();
}

bool OrderPumpWater::IsIdleOrder() noexcept
{
	return false;
}

void OrderPumpWater::RunParallel()
{
	OrderWalk::RunParallel();
}

bool OrderPumpWater::CanExecuteIfHurt() noexcept
{
	return false;
}