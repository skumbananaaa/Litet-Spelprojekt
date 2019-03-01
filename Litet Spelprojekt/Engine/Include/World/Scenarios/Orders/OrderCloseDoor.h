#pragma once
#include <EnginePch.h>
#include <World/World.h>
#include <Graphics/Scene.h>
#include <World/Scenarios/IOrder.h>


class API OrderCloseDoor : public IOrder
{
protected:
	OrderCloseDoor();

public:
	virtual ~OrderCloseDoor();

	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;

};