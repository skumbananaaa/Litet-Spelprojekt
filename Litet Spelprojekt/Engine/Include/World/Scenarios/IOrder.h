#pragma once
#include <EnginePch.h>
#include <World/World.h>
#include <Graphics/Scene.h>
#include "../../../../../Game/Include/Crew.h"

class API IOrder
{
protected:
	IOrder();

public:
	virtual ~IOrder();

	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
	virtual void UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
private:

};