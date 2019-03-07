#include "../Include/GameObjectFloor.h"
#include "../Include/Game.h"
#include "../Include/Orders/OrderExtinguishFire.h"
#include <World/World.h>

GameObjectFloor::GameObjectFloor()
{
}

GameObjectFloor::~GameObjectFloor()
{
}

int32 GameObjectFloor::TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float extension) noexcept
{
	glm::vec3 centre = GetPosition();
	centre.x += extension * glm::floor(centre.y / 2.0f);

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

	float t = -1;
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

	return t;
}

void GameObjectFloor::OnPicked(const std::vector<int32>& selectedMembers, int32 x, int32 y) noexcept
{
	std::cout << "I am a picked floor!" << std::endl;
	const glm::ivec3& tile = GetTile();
	TileData* pLowerTile = &Game::GetGame()->m_pSceneGame->GetWorld()->GetLevel(tile.y).GetLevelData()[tile.x][tile.z];
	TileData* pUpperTile = &Game::GetGame()->m_pSceneGame->GetWorld()->GetLevel(tile.y + 1).GetLevelData()[tile.x][tile.z];

	if (pLowerTile->Temp > pLowerTile->BurnsAt)
	{
		AddChoice("Extinguish Fire", nullptr);
	}

	//if (!IsOpen())
	//{
	//	AddChoice("�ppna", reinterpret_cast<void*>(true));
	//}

	DisplayOrders(x, y, selectedMembers);
}

void GameObjectFloor::OnAddedToScene(Scene* scene) noexcept
{
	scene->RegisterPickableGameObject(this);
}

void GameObjectFloor::OnOrderChosen(const std::string& name, void* userData, const std::vector<int32>& selectedMembers) noexcept
{
	Crew* crew = Game::GetGame()->m_pSceneGame->GetCrew();
	for (uint32 i = 0; i < selectedMembers.size(); i++)
	{
		const glm::ivec3& tile = GetTile();
		Crewmember* pCrewmember = crew->GetMember(selectedMembers[i]);
		pCrewmember->GiveOrder(new OrderExtinguishFire(glm::ivec3(1, 4, 1), tile,
			Game::GetGame()->m_pSceneGame->GetWorld()->GetLevel(tile.y).GetLevel()[tile.x][tile.z], pCrewmember->HasGearEquipped()));
	}
}