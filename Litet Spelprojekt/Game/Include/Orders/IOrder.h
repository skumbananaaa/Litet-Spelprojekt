#pragma once
#include <World/World.h>
#include <Graphics/Scene.h>
#include "../../Include/Crew.h"

class IOrder
{
	friend class OrderHandler;

protected:
	IOrder() : m_pCrewMember(nullptr) {};
	Crewmember* GetCrewMember() noexcept 
	{
		return m_pCrewMember;
	};

public:
	virtual ~IOrder() {};

	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
	virtual bool UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept = 0;

private:
	Crewmember* m_pCrewMember;
};