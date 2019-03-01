#include "../../Include/Orders/OrderCloseDoor.h"

OrderCloseDoor::OrderCloseDoor()
{

}

OrderCloseDoor::~OrderCloseDoor()
{
}

void OrderCloseDoor::StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers)
{

	/*std::vector<uint32> crewRoomIndexArray;
	std::vector<uint32> doorRoomIndexArray;
	std::vector<glm::ivec2> roomInCommon;
	glm::ivec3 playerTile = m_pCrewMember->GetTile();

	uint32 crewRoomIndex = pWorld->GetLevel(playerTile.y * 2)->GetLevel()[playerTile.x][playerTile.z];
	crewRoomIndexArray.push_back(crewRoomIndex);

	for (int i = 0; i < pWorld->GetNumDoors(); i++)
	{
		glm::ivec3 doorTile = pWorld->GetDoor(i);
		uint32 doorRoomIndex = pWorld->GetLevel(doorTile.y)->GetLevel()[doorTile.x][doorTile.z];
		//doorRoomIndexArray.push_back(doorRoomIndex);

		if (doorRoomIndex == crewRoomIndex)
		{
			//här vill jag pusha till orderPool.
			roomInCommon.push_back(glm::ivec2(crewRoomIndex, doorRoomIndex));

		}
		
	}*/
}

bool OrderCloseDoor::UpdateOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers, float dtS) noexcept
{
	return false;
}

void OrderCloseDoor::EndOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
}

