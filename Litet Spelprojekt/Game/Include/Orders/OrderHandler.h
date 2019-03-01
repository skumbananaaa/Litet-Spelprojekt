#pragma once
#include "IOrder.h"

class OrderHandler
{
public:
	static void GiveOrder(IOrder* order, Crewmember* crewMember) noexcept;
	static void Update(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept;
	static void Release() noexcept;
private:
	OrderHandler(); 
	static std::vector<IOrder*> s_OrderQueue;
};