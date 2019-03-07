#include <EnginePch.h>
#include "../../Include/Orders/OrderWalk.h"
#include <World/World.h>
#include <Graphics/Scene.h>
#include "../../Include/Crew.h"
#include "../../Include/Path.h"
#include "../../Include/GameObjectDoor.h"
#include <System/Random.h>

OrderWalk::OrderWalk(glm::ivec3 goalTile):
	m_pPathFinder(nullptr),
	m_pPath(nullptr),
	m_IsPathReady(false)
{
	m_GoalTile = glm::ivec3(goalTile.x, goalTile.y / 2, goalTile.z);
}

OrderWalk::~OrderWalk()
{
}

void OrderWalk::StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	m_pPathFinder = new Path(pWorld);
	GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_RUN);
	ThreadHandler::RequestExecution(this);
}

bool OrderWalk::UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	if (!m_IsPathReady)
	{
		return false;
	}

	TileData& tile1 = pWorld->GetLevel(GetCrewMember()->GetTile().y * 2).GetLevelData()[GetCrewMember()->GetTile().x][GetCrewMember()->GetTile().z];
	GameObjectDoor* door1 = (GameObjectDoor*)tile1.GameObjects[GAMEOBJECT_CONST_INDEX_DOOR];
	TileData& tile2 = pWorld->GetLevel(m_TargetTile.y * 2).GetLevelData()[m_TargetTile.x][m_TargetTile.z];
	GameObjectDoor* door2 = (GameObjectDoor*)tile2.GameObjects[GAMEOBJECT_CONST_INDEX_DOOR];

	if (door1)
	{
		if (door1 == door2)
		{
			// Open door before passing through
			if (!door1->IsOpen() && !door2->IsOpen() && GetCrewMember()->GetTile() != m_TargetTile)
			{
				if (door1->IsClosed())
				{
					GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_OPENDOOR);
					door1->SetOpen(true);
				}
				return false;
			}
			else
			{
				GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_RUN);
			}
		}
		else if ((!door1->IsClosed() && GetCrewMember()->GetTile() != m_TargetTile) && (m_OopsIForgot > GetCrewMember()->GetForgetfulness()))
		{
			// Close door after passing through
			if (door1->IsOpen())
			{
				GetCrewMember()->SetDirection(-GetCrewMember()->GetDirection());
				GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_OPENDOOR);
				door1->SetOpen(false);
			}
			return false;
		}
		else
		{
			GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_RUN);
		}
	}

	Room& room = pWorld->GetRoom(pWorld->GetLevel(GetCrewMember()->GetTile().y * 2).GetLevel()[GetCrewMember()->GetTile().x][GetCrewMember()->GetTile().z]);

	if (room.IsBurning())
	{
		if (!room.IsFireDetected())
		{
			room.SetFireDetected(true);
		}
		GetCrewMember()->GiveOrder(new OrderWalk(m_GoalTile * glm::ivec3(1, 2, 1)));
	}

	if (room.IsFlooded() && !room.IsFloodDetected())
	{
		room.SetFloodDetected(true);
		GetCrewMember()->GiveOrder(new OrderWalk(m_GoalTile * glm::ivec3(1, 2, 1)));
	}

	return FollowPath(dtS);
}

void OrderWalk::EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_IDLE);
	DeleteSafe(m_pPathFinder);
}

void OrderWalk::AbortOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	DeleteSafe(m_pPathFinder);
}

bool OrderWalk::AllowsMultipleOrders() noexcept
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
	const glm::vec3& position = GetCrewMember()->GetPosition();
	if (m_NrOfTilesLeft > 0)
	{
		if (GetCrewMember()->GetTile() == m_TargetTile)
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
			GetCrewMember()->SetDirection(glm::vec3(1, 0, 0));
			GetCrewMember()->Move(glm::vec3(0, move.y, 0), false, dtS);
		}
		else
		{
			GetCrewMember()->SetDirection(glm::vec3(move.x, 0, move.z));
			GetCrewMember()->Move(GetCrewMember()->GetDirection(), true, dtS);
		}
	}
	else
	{
		return true;
	}

	return false;
}
