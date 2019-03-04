#pragma once
#include "IOrder.h"
#include <vector>

class OrderHandler
{
public:
	OrderHandler();
	~OrderHandler();

	void GiveOrder(IOrder* order, Crewmember* crewMember) noexcept;
	void Update(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept;

private:
	std::vector<IOrder*> s_OrderQueue;
};