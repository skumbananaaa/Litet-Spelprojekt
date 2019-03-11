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
	IOrder() : m_pCrewMember(nullptr){};
	Crewmember* GetCrewMember() noexcept 
	{
		return m_pCrewMember;
	};

public:
	virtual ~IOrder() {};

	virtual void OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
	virtual bool OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept = 0;
	virtual void OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
	virtual bool CanBeStackedWithSameType() noexcept = 0;
	virtual bool HasPriority() noexcept = 0;
	virtual std::string GetName() noexcept = 0;
	virtual bool ReadyToAbort() noexcept = 0;
	virtual bool IsIdleOrder() noexcept = 0;
	virtual bool CanExecuteIfHurt() noexcept = 0;

private:
	Crewmember* m_pCrewMember;
};