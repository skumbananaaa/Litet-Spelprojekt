#include <EnginePch.h>
#include "../../Include/Orders/OrderWalk.h"
#include <World/World.h>
#include <Graphics/Scene.h>
#include "../../Include/Crew.h"
#include "../../Include/Path.h"
#include "../../Include/GameObjectDoor.h"
#include <World/Logger.h>
#include <System/Random.h>

OrderWalk::OrderWalk(const glm::ivec3& goalTile):
	m_pPathFinder(nullptr),
	m_pPath(nullptr),
	m_IsPathReady(false)
{
	m_GoalTile = glm::ivec3(goalTile.x, goalTile.y / 2, goalTile.z);
}

OrderWalk::~OrderWalk()
{
	DeleteSafe(m_pPathFinder);
}

void OrderWalk::OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	Crewmember* pCrewmember = GetCrewMember();
	m_pPathFinder = new Path(pWorld, pCrewmember->GetGroupType() == SMOKE_DIVER && pCrewmember->HasGearEquipped());
	
	if (pCrewmember->HasExtinguisherEquipped())
	{
		pCrewmember->SetMaterial(MATERIAL::ANIMATED_MODEL_EXTINGUISH);
		pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_EXTINGUISH_RUN);
	}
	else
	{
		pCrewmember->SetMaterial(MATERIAL::ANIMATED_MODEL);
		if (pCrewmember->IsIdling())
		{
			pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_WALK);
		}
		else
		{
			pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_RUN);
		}
	}

	ThreadHandler::RequestExecution(this);
}

bool OrderWalk::OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	if (!m_IsPathReady)
	{
		return false;
	}

	Crewmember* pCrewmember = GetCrewMember();
	const glm::ivec3& crewTile = pCrewmember->GetTile();

	TileData& tile1 = pWorld->GetLevel(crewTile.y * 2).GetLevelData()[crewTile.x][crewTile.z];
	GameObjectDoor* door1 = (GameObjectDoor*)tile1.GameObjects[GAMEOBJECT_CONST_INDEX_DOOR];

	TileData& tile2 = pWorld->GetLevel(m_TargetTile.y * 2).GetLevelData()[m_TargetTile.x][m_TargetTile.z];
	GameObjectDoor* door2 = (GameObjectDoor*)tile2.GameObjects[GAMEOBJECT_CONST_INDEX_DOOR];

	uint32 index1 = pWorld->GetLevel(crewTile.y * 2).GetLevel()[crewTile.x][crewTile.z];
	uint32 index2 = pWorld->GetLevel(m_TargetTile.y * 2).GetLevel()[m_TargetTile.x][m_TargetTile.z];

	if (door1)
	{
		if (door1 == door2)
		{
			// Open door before passing through
			if (!door1->IsOpen() && crewTile != m_TargetTile)
			{
				//if (door1->IsClosed())
				{
					if (door1->AccessRequest(pCrewmember->GetShipNumber()))
					{
						pCrewmember->SetMaterial(MATERIAL::ANIMATED_MODEL);
						pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_OPENDOOR);
						door1->SetOpen(true);
					}
				}
				/*else*/ if (pCrewmember->GetAnimatedMesh() != ResourceHandler::GetAnimatedMesh(MESH::ANIMATED_MODEL_OPENDOOR))
				{
					pCrewmember->SetMaterial(MATERIAL::ANIMATED_MODEL);
					pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_IDLE);
				}
				return false;
			}
			else
			{
				if (pCrewmember->HasExtinguisherEquipped())
				{
					pCrewmember->SetMaterial(MATERIAL::ANIMATED_MODEL_EXTINGUISH);
					pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_EXTINGUISH_RUN);
				}
				else if (pCrewmember->IsIdling())
				{
					pCrewmember->SetMaterial(MATERIAL::ANIMATED_MODEL);
					pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_WALK);
				}
				else
				{
					pCrewmember->SetMaterial(MATERIAL::ANIMATED_MODEL);
					pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_RUN);
				}
			}
		}
		else if (door1->RemoveFromQueue(pCrewmember->GetShipNumber()) && !door1->IsClosed() && m_OopsIForgot > pCrewmember->GetForgetfulness())
		{
			// Close door after passing through
			door1->AccessRequest(pCrewmember->GetShipNumber());
			if (door1->IsOpen())
			{
				pCrewmember->SetDirection(-pCrewmember->GetDirection());
				pCrewmember->SetMaterial(MATERIAL::ANIMATED_MODEL);
				pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_OPENDOOR);
				door1->SetOpen(false);
			}
			return false;
		}
		else
		{
			if (pCrewmember->HasExtinguisherEquipped())
			{
				pCrewmember->SetMaterial(MATERIAL::ANIMATED_MODEL_EXTINGUISH);
				pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_EXTINGUISH_RUN);
			}
			else if (pCrewmember->IsIdling())
			{
				pCrewmember->SetMaterial(MATERIAL::ANIMATED_MODEL);
				pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_WALK);
			}
			else
			{
				pCrewmember->SetMaterial(MATERIAL::ANIMATED_MODEL);
				pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_RUN);
			}
		}
	}

	Room& room = pWorld->GetRoom(pWorld->GetLevel(crewTile.y * 2).GetLevel()[crewTile.x][crewTile.z]);

	if (room.IsActive())
	{
		pCrewmember->UpdateLastKnownPosition();
	}
	
	if (m_NewRoom && index1 == index2)
	{
		m_NewRoom = false;
		pCrewmember->LookForDoor();

		if (room.IsFireDetected())
		{
			pCrewmember->GiveOrder(new OrderWalk(m_GoalTile * glm::ivec3(1, 2, 1)));
		}

		if (room.IsFloodDetected())
		{
			pCrewmember->GiveOrder(new OrderWalk(m_GoalTile * glm::ivec3(1, 2, 1)));
		}
	}

	m_NewRoom = index1 != index2;
	
	return FollowPath(dtS);
}

void OrderWalk::OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	Crewmember* pCrewmember = GetCrewMember();

	//pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_IDLE);
}

bool OrderWalk::CanBeStackedWithSameType() noexcept
{
	return false;
}

bool OrderWalk::HasPriority() noexcept
{
	return false;
}

std::string OrderWalk::GetName() noexcept
{
	return "Walk";
}

bool OrderWalk::ReadyToAbort() noexcept
{
	return m_IsPathReady;
}

bool OrderWalk::IsIdleOrder() noexcept
{
	return false;
}

void OrderWalk::RunParallel()
{
	m_pPath = m_pPathFinder->FindPath(GetCrewMember()->GetTile(), m_GoalTile);
	m_NrOfTilesLeft = m_pPathFinder->GetNrOfPathTiles();
	m_TargetTile = GetCrewMember()->GetTile();
	m_TargetPos = glm::vec3(m_TargetTile.x, m_TargetTile.y * 2, m_TargetTile.z);
	m_IsPathReady = true;
}

bool OrderWalk::CanExecuteIfHurt() noexcept
{
	return true;
}

bool OrderWalk::FollowPath(float dtS) noexcept
{
	Crewmember* pCrewmember = GetCrewMember();

	const glm::vec3& position = pCrewmember->GetPosition();
	if (m_NrOfTilesLeft > 0)
	{
		if (pCrewmember->GetTile() == m_TargetTile)
		{
			m_directionTile = m_pPath[m_NrOfTilesLeft - 1];
			m_TargetTile = m_pPath[--m_NrOfTilesLeft];
			m_TargetPos = glm::vec3(m_TargetTile.x, m_TargetTile.y * 2, m_TargetTile.z);
			m_OopsIForgot = Random::GenerateInt(0, 100);
		}
	}

	if ((std::abs(position.x - m_TargetPos.x) > 0.1 || std::abs(position.y - m_TargetPos.y) > 0.1 || std::abs(position.z - m_TargetPos.z) > 0.1))
	{
		glm::vec3 move = m_TargetPos - position;
		move = glm::normalize(move);
		if (std::abs(move.y) > 0.1)
		{
			move.y /= std::abs(move.y);
			pCrewmember->SetDirection(glm::vec3(1, 0, 0));
			pCrewmember->Move(glm::vec3(0, move.y, 0), false, dtS);
		}
		else
		{
			pCrewmember->SetDirection(glm::vec3(move.x, 0, move.z));
			pCrewmember->Move(pCrewmember->GetDirection(), true, dtS);
			pCrewmember->SetPosition(glm::vec3(pCrewmember->GetPosition().x, m_TargetPos.y, pCrewmember->GetPosition().z));
		}
	}
	else
	{
		return true;
	}

	return false;
}
