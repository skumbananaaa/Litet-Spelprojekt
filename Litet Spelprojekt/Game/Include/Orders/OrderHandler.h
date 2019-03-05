#pragma once
#include "IOrder.h"
#include <vector>

class IOrderListener
{
public:
	virtual void OnOrderRequested() noexcept = 0;
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
	std::vector<IOrder*> m_OrderQueue;
	IOrderListener* m_pOrderListener;
};