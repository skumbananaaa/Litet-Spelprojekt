#include "../Include/GameObjectFloor.h"
#include "../Include/Game.h"
#include "../Include/Orders/OrderExtinguishFire.h"
#include "../Include/Orders/OrderPlugHole.h"
#include "../Include/Orders/OrderPumpWater.h"
#include <World/World.h>

GameObjectFloor::GameObjectFloor()
{
}

GameObjectFloor::~GameObjectFloor()
{
}

int32 GameObjectFloor::TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float elevation, float extension) noexcept
{
	glm::vec3 centre = GetPosition();
	centre.x += extension * glm::floor(centre.y / 2.0f);

	float t = -1;

	if (glm::floor(centre.y / 2) == glm::floor(elevation / 2) || extension > 0)
	{
		glm::vec3 normals[]{
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		};

		float h[] = {
			GetScale().x / 2.0f,
			GetScale().y / 2.0f,
			GetScale().z / 2.0f
		};

		float d1[] = {
			glm::dot(centre - normals[0] * h[0], normals[0]),
			glm::dot(centre - normals[1] * h[1], normals[1]),
			glm::dot(centre - normals[2] * h[2], normals[2])
		};
		float d2[] = {
			glm::dot(centre + normals[0] * h[0], normals[0]),
			glm::dot(centre + normals[1] * h[1], normals[1]),
			glm::dot(centre + normals[2] * h[2], normals[2])
		};

		float t1[3];
		float t2[3];
		float t_min[3];
		float t_max[3];

		float min_t, max_t;

		for (int j = 0; j < 3; j++)
		{
			if (std::abs(glm::dot(normals[j], ray)) > 0.01)
			{
				t1[j] = (d1[j] - glm::dot(normals[j], origin)) / glm::dot(normals[j], ray);
				t2[j] = (d2[j] - glm::dot(normals[j], origin)) / glm::dot(normals[j], ray);

				t_min[j] = std::min(t1[j], t2[j]);
				t_max[j] = std::max(t1[j], t2[j]);
			}
			else if (-glm::dot(normals[0], centre - origin) - h[j] > 0 || -glm::dot(normals[0], centre - origin) + h[j] < 0)
				return -1;
		}

		min_t = std::max(t_min[0], t_min[1]);
		min_t = std::max(min_t, t_min[2]);
		max_t = std::min(t_max[0], t_max[1]);
		max_t = std::min(max_t, t_max[2]);

		if (min_t <= max_t && max_t >= 0)
		{
			if (t_min > 0)
				t = min_t;
			else
				t = max_t;
		}
	}

	return (int32)t;
}

void GameObjectFloor::OnPicked(const std::vector<int32>& selectedMembers, int32 x, int32 y) noexcept
{
	std::cout << "I am a picked floor!" << std::endl;
	const glm::ivec3& tile = GetTile();
	TileData* pLowerTile = &Game::GetGame()->m_pSceneGame->GetWorld()->GetLevel(tile.y).GetLevelData()[tile.x][tile.z];

	if (pLowerTile->Temp > pLowerTile->BurnsAt)
	{
		AddChoice("Släck eld", nullptr);
	}
	if (pLowerTile->WaterInlet)
	{
		AddChoice("Plugga hål", nullptr);
	}
	else if (pLowerTile->AlreadyFlooded)
	{
		AddChoice("Pumpa vatten", nullptr);
	}

	DisplayOrders(x, y, selectedMembers);
}

void GameObjectFloor::OnAddedToScene(Scene* scene) noexcept
{
	scene->RegisterPickableGameObject(this);
}

void GameObjectFloor::OnOrderChosen(const std::string& name, void* userData, const std::vector<int32>& selectedMembers) noexcept
{
	Crew* pCrew = Game::GetGame()->m_pSceneGame->GetCrew();
	World* pWorld = Game::GetGame()->m_pSceneGame->GetWorld();
	std::vector<GameObject*> pExtinguishers;

	if (name == "Släck eld")
	{
		const glm::ivec3& tile = GetTile();
		glm::ivec3 extinguisherTile(-1);
		for (uint32 i = 0; i < selectedMembers.size(); i++)
		{
			Crewmember* pCrewmember = pCrew->GetMember(selectedMembers[i]);
	

			bool hasGearEquipped = pCrewmember->HasGearEquipped();
			bool hasExtinguisherEquipped = pCrewmember->HasExtinguisherEquipped();
			glm::ivec3 goalTile;

			if (!hasGearEquipped)
			{
				goalTile = pWorld->FindClosestRoomInInterval(CABOOSE_INTERVAL_START, CABOOSE_INTERVAL_END, tile, true);
			}
			else
			{
				if (!hasExtinguisherEquipped)
				{
					goalTile = FindClosestExtinguisher(pCrewmember->GetPosition());
				}
				else
				{
					goalTile = tile;
				}
			}

			pCrewmember->GiveOrder(new OrderExtinguishFire(
				goalTile,
				tile,
				pWorld->GetLevel(tile.y).GetLevel()[tile.x][tile.z],
				false, ""));
		}
	}
	else if (name == "Plugga hål")
	{
		const glm::ivec3& tile = GetTile();
		for (uint32 i = 0; i < selectedMembers.size(); i++)
		{
			Crewmember* pCrewmember = pCrew->GetMember(selectedMembers[i]);
			pCrewmember->GiveOrder(new OrderPlugHole(
				pWorld->FindClosestRoomInInterval(CABOOSE_INTERVAL_START, CABOOSE_INTERVAL_END, tile),
				tile,
				pCrewmember->HasGearEquipped()));
		}
	}
	else if (name == "Pumpa vatten")
	{
		const glm::ivec3& tile = GetTile();
		bool res = false;
		Crewmember* pCrewMember = nullptr;
		uint32 roomId = pWorld->GetLevel(tile.y).GetLevel()[tile.x][tile.z];
		if (!pWorld->GetRoom(roomId).IsPumping())
		{
			for (uint32 i = 0; i < selectedMembers.size(); i++)
			{
				pCrewMember = pCrew->GetMember(selectedMembers[i]);
				if (pCrewMember->IsIdling() && !res)
				{
					res = true;
					pCrewMember->GiveOrder(new OrderPumpWater(roomId,
						pWorld->FindClosestRoomInInterval(MACHINE_ROOM_INTERVAL_START, MACHINE_ROOM_INTERVAL_END, pCrewMember->GetTile())));
					break;
				}
			}
			if (!res && pCrewMember != nullptr)
			{
				pCrewMember->GiveOrder(new OrderPumpWater(roomId,
					pWorld->FindClosestRoomInInterval(MACHINE_ROOM_INTERVAL_START, MACHINE_ROOM_INTERVAL_END, pCrewMember->GetTile())));
			}
		}
	}
	/*if (reinterpret_cast<int>(userData) == 1)
	{
		Logger::LogEvent("YAAS", true);
	}*/
}
