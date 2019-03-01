#include <EnginePch.h>
#include <World/Scenarios/Orders/OrderCloseDoor.h>

void OrderCloseDoor::StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers)
{

	std::vector<uint32> crewRoomIndexArray;
	std::vector<uint32> doorRoomIndexArray;
	std::vector<glm::ivec2> roomInCommon;
	glm::ivec3 playerTile(0.0f);

	uint32 crewRoomIndex = pWorld->GetLevel(m_PlayerTile.y * 2)->GetLevel()[m_PlayerTile.x][m_PlayerTile.z];
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

	}
}

