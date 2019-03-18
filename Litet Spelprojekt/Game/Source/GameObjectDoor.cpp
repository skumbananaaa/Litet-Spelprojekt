#include <EnginePch.h>
#include "../Include/GameObjectDoor.h"
#include "../Include/Game.h"
#include "../Include/Orders/OrderCloseDoor.h"
#include <Graphics/Scene.h>

GameObjectDoor::GameObjectDoor() noexcept
{
	//Set doors to be updated
	m_IsTickable = true;

	UpdateTransform();

	m_Percentage = 0.0f;
	m_Open = true;
}

GameObjectDoor::~GameObjectDoor()
{

}

void GameObjectDoor::SetOpen(bool open) noexcept
{
	m_Open = open;
}

bool GameObjectDoor::IsOpen() const noexcept
{
	return m_Percentage >= 1.0F;
}

bool GameObjectDoor::IsClosed() const noexcept
{
	return m_Percentage <= 0.0F;
}

bool GameObjectDoor::AccessRequest(uint32 shipNr) noexcept
{
	if (std::find(m_AccessQueue.begin(), m_AccessQueue.end(), shipNr) == m_AccessQueue.end())
	{
		m_AccessQueue.push_back(shipNr);
		return m_AccessQueue.size() == 1;
	}
	return false;
}

bool GameObjectDoor::RemoveFromQueue(uint32 shipNr) noexcept
{
	std::vector<uint32>::iterator it = std::find(m_AccessQueue.begin(), m_AccessQueue.end(), shipNr);
	if (it != m_AccessQueue.end())
	{
		m_AccessQueue.erase(it);
		return m_AccessQueue.empty();
	}
	return false;
}

DOOR_COLOR GameObjectDoor::GetColor() const noexcept
{
	if (GetMaterial() == ResourceHandler::GetMaterial(MATERIAL::DOOR_RED))
	{
		return DOOR_COLOR_RED;
	}
	else if (GetMaterial() == ResourceHandler::GetMaterial(MATERIAL::DOOR_GREEN))
	{
		return DOOR_COLOR_GREEN;
	}
	else if (GetMaterial() == ResourceHandler::GetMaterial(MATERIAL::DOOR_BLUE))
	{
		return DOOR_COLOR_BLUE;
	}
	else if (GetMaterial() == ResourceHandler::GetMaterial(MATERIAL::DOOR_YELLOW))
	{
		return DOOR_COLOR_YELLOW;
	}
	return DOOR_COLOR_YELLOW;
}

void GameObjectDoor::Update(const Camera& camera, float deltaTime) noexcept
{
	GameObject::Update(camera, deltaTime);

	//Test
	/*if (IsOpen())
	{
		SetOpen(false);
	}
	else if(IsClosed())
	{
		SetOpen(true);
	}*/


	const float lastPercentage = m_Percentage;
	m_Percentage += deltaTime * (m_Open * 2 - 1);
	m_Percentage = glm::clamp<float>(m_Percentage, 0.0F, 1.0F);
	if (m_Percentage <= 0.0f)
	{
		m_AccessQueue.clear();
	}
	if (lastPercentage != m_Percentage)
	{
		m_IsDirty = true;
		UpdateTransform();
	}
}

int32 GameObjectDoor::TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float elevation, float extension) noexcept
{
	glm::vec3 centre = GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f);
	centre.x += extension * glm::floor(centre.y / 2.0f);

	float t = -1;

	if (glm::floor(centre.y / 2) == glm::floor(elevation / 2) || extension > 0)
	{
		glm::vec3 normals[]{
			m_Direction,
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::normalize(glm::cross(normals[0], normals[1]))
		};

		float h[] = {
			0.15,
			1.0,
			0.5
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

	return t;
}

void GameObjectDoor::UpdateTransform() noexcept
{
	if (m_IsDirty && m_IsVisible)
	{
		const glm::vec4& rotation = GetRotation();
		glm::mat4 transform(1.0f);

		transform = glm::translate(transform, GetPosition()); // 5. Translate to world position
		transform = glm::rotate(transform, rotation.w, glm::vec3(rotation.x, rotation.y, rotation.z)); // 4. Rotate To Align with the walls
		transform = glm::translate(transform, glm::vec3(0.0F, 0.0F, 0.5F)); // 3. Translate To Origin
		transform = glm::rotate(transform, m_Percentage * glm::half_pi<float>(), glm::vec3(0.0F, 1.0f, 0.0F)); // 2. Rotate along the hinge
		transform = glm::translate(transform, glm::vec3(0.0F, 0.0F, -0.5F)); // 1. Translate To The Edge

		transform = glm::scale(transform, GetScale());
		m_transform = transform;

		m_IsDirty = false;
	}
}

void GameObjectDoor::OnPicked(const std::vector<int32>& selectedMembers, int32 x, int32 y) noexcept
{
	std::cout << "I am a picked door!" << std::endl;

	if (!IsClosed())
	{
		AddChoice("Stäng", reinterpret_cast<void*>(false));
	}
	else if (!IsOpen())
	{
		AddChoice("Öppna", reinterpret_cast<void*>(true));
	}

	DisplayOrders(x, y, selectedMembers);
}

void GameObjectDoor::OnAddedToScene(Scene* scene) noexcept
{
	scene->RegisterPickableGameObject(this);
}

void GameObjectDoor::OnOrderChosen(const std::string& name, void* userData, const std::vector<int32>& selectedMembers) noexcept
{
	Crew* crew = Game::GetGame()->m_pSceneGame->GetCrew();
	float shortDistance = FLT_MAX;
	int32 shipID = -1;
	for (int i = 0; i < selectedMembers.size(); i++)
	{
		float distance = glm::distance(GetPosition(), crew->GetMember(selectedMembers[i])->GetPosition());
		if (distance < shortDistance)
		{
			shortDistance = distance;
			shipID = selectedMembers[i];
		}
	}

	if (shipID >= 0)
	{
		World* pWorld = Game::GetGame()->m_pSceneGame->GetWorld();
		const glm::ivec3& tile = GetTile();
		glm::ivec3 tile2;
		TileData * const * tiles = pWorld->GetLevel(tile.y).GetLevelData();
		Crewmember* crewmember = crew->GetMember(shipID);

		if (tiles[tile.x + 1][tile.z].HasDoor())
		{
			tile2 = tile;
			tile2.x += 1;
		}
		else if (tiles[tile.x - 1][tile.z].HasDoor())
		{
			tile2 = tile;
			tile2.x -= 1;
		}
		else if (tiles[tile.x][tile.z + 1].HasDoor())
		{
			tile2 = tile;
			tile2.z += 1;
		}
		else if (tiles[tile.x][tile.z - 1].HasDoor())
		{
			tile2 = tile;
			tile2.z -= 1;
		}

		if (glm::distance((glm::vec3)tile, crewmember->GetPosition()) < glm::distance((glm::vec3)tile2, crewmember->GetPosition()))
		{
			tile2 = tile;
		}

		crewmember->GiveOrder(new OrderDoor(this, tile2, reinterpret_cast<uint32>(userData)));
	}
}
