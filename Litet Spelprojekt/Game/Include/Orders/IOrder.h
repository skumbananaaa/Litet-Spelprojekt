#pragma once
#include <string>

class Scene;
class World;
class Crew;
class Crewmember;

class IOrder
{
	friend class OrderHandler;

protected:
	IOrder() : m_pCrewMember(nullptr), m_Abort(false){};
	Crewmember* GetCrewMember() noexcept 
	{
		return m_pCrewMember;
	};

public:
	virtual ~IOrder() {};

	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
	virtual bool UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept = 0;
	virtual void AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
	virtual bool AllowsMultipleOrders() noexcept = 0;
	virtual std::string GetName() noexcept = 0;
	virtual bool ReadyToAbort() noexcept = 0;

private:
	Crewmember* m_pCrewMember;
	bool m_Abort;
};