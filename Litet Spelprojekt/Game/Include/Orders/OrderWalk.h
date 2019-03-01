#pragma once
#include "IOrder.h"
#include <System/ThreadHandler.h>
#include <GLM/glm.hpp>

class Path;

class OrderWalk : public IOrder, public IRunnable
{
protected:

public:
	OrderWalk(glm::ivec3 targetTile);
	virtual ~OrderWalk();

	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool AllowsMultipleOrders() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool ReadyToAbort() noexcept override;
	virtual void RunParallel() override;

protected:
	bool FollowPath(float dtS) noexcept;
protected:
	glm::ivec3 m_GoalTile;
	glm::vec3 m_directionTile;
	glm::ivec3 m_TargetTile;
	glm::vec3 m_TargetPos;
	Path* m_pPathFinder;
	glm::ivec3* m_pPath;
	int32 m_NrOfTilesLeft;
	bool m_IsPathReady;
};