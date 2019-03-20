#pragma once
#include "IOrder.h"
#include <System/ThreadHandler.h>
#include <GLM/glm.hpp>
#include <World/Scenarios/Fire/FireAlarm.h>
#include <World/Logger.h>
#include "../../Include/Crew.h"

class Path;

class OrderWalk : public IOrder, public IRunnable
{
public:
	OrderWalk(OrderWalk* other);
	OrderWalk(const glm::ivec3& goalTile);
	virtual ~OrderWalk();

	virtual void OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void OnAborted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual bool CanBeStackedWithSameType() noexcept override;
	virtual bool HasPriority() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool ReadyToAbort() noexcept override;
	virtual bool IsIdleOrder() noexcept override;
	virtual void RunParallel() override;
	virtual bool CanExecuteIfHurt() noexcept override;

	virtual IOrder* Clone() noexcept override;

	void SetAlternateMeshAndMaterial(Crewmember* pCrewmember, int32 mesh, int32 material) noexcept;

protected:
	bool FollowPath(float dtS) noexcept;

private:
	void SetMeshAndMaterial(Crewmember* pCrewmember, int32 mesh, int32 material) noexcept;

protected:
	glm::ivec3 m_GoalTile;
	glm::vec3 m_directionTile;
	glm::ivec3 m_TargetTile;
	glm::vec3 m_TargetPos;
	Path* m_pPathFinder;
	glm::ivec3* m_pPath;
	int32 m_NrOfTilesLeft;
	bool m_IsPathReady;
	int32 m_OopsIForgot;
	int32 m_AlternateMesh;
	int32 m_AlternateMaterial;

	bool m_NewRoom = false;
};

inline void OrderWalk::SetAlternateMeshAndMaterial(Crewmember* pCrewmember, int32 mesh, int32 material) noexcept
{
	m_AlternateMesh = mesh;
	m_AlternateMaterial = material;
	pCrewmember->UpdateAnimatedMesh(m_AlternateMesh);
	pCrewmember->SetMaterial(m_AlternateMaterial);
}

inline void OrderWalk::SetMeshAndMaterial(Crewmember* pCrewmember, int32 mesh, int32 material) noexcept
{
	if (m_AlternateMesh < 0)
	{
		pCrewmember->UpdateAnimatedMesh(mesh);
	}
	else
	{
		pCrewmember->UpdateAnimatedMesh(m_AlternateMesh);
	}

	if (m_AlternateMaterial < 0)
	{
		pCrewmember->SetMaterial(material);
	}
	else
	{
		pCrewmember->SetMaterial(m_AlternateMaterial);
	}
}


