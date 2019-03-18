#pragma once
#include <EnginePch.h>
#include <string>
#include "../IReplayable.h"

class Scene;
class World;
class Crew;
class Crewmember;

class IOrder : public IReplayable
{
	friend class OrderHandler;

protected:
	IOrder(IOrder* other);
	IOrder() : m_pCrewMember(nullptr), m_IsInbred(false), m_IsAborted(false){};
	Crewmember* GetCrewMember() noexcept 
	{
		return m_pCrewMember;
	};

public:
	virtual ~IOrder() {};

	virtual void OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
	virtual bool OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept = 0;
	virtual void OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
	virtual void OnAborted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
	virtual bool CanBeStackedWithSameType() noexcept = 0;
	virtual bool HasPriority() noexcept = 0;
	virtual std::string GetName() noexcept = 0;
	virtual bool ReadyToAbort() noexcept = 0;
	virtual bool IsIdleOrder() noexcept = 0;
	virtual bool CanExecuteIfHurt() noexcept = 0;

	virtual IOrder* Clone() noexcept = 0;
	virtual void InitClone(SceneGame* pScene, void* userData) noexcept;
	virtual void BeginReplay(SceneGame* pScene, void* userData) noexcept override;
	virtual bool IsDeleteable() const noexcept override { return true; }
protected:
	void GiveOrderInbred(IOrder* order) noexcept;

private:
	Crewmember* m_pCrewMember;

	//Replay
	uint32 m_ShipId;
	bool m_IsInbred;
	bool m_IsAborted;
};