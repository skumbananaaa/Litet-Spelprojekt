#pragma once
#include "IOrder.h"
#include <vector>

class IOrderListener
{
public:
	virtual ~IOrderListener() {};
	virtual void OnOrderStarted(bool idleOrder) noexcept = 0;
	virtual void OnAllOrdersFinished() noexcept = 0;
};

class OrderHandler
{
public:
	OrderHandler(IOrderListener* pOrderListener);
	~OrderHandler();

	void GiveOrder(IOrder* order, Crewmember* crewMember) noexcept;
	void Update(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept;

private:
	void StartOrder();

	std::vector<IOrder*> m_OrderQueue;
	IOrderListener* m_pOrderListener;
};