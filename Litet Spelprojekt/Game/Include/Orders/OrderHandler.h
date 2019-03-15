#pragma once
#include "IOrder.h"
#include <vector>
#include <Defines.h>

class OrderHandler
{
	friend class IOrder;

public:
	OrderHandler(Crewmember* pCrewMember);
	~OrderHandler();

	void GiveOrder(IOrder* order) noexcept;
	void Update(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept;

	static void Reset() noexcept;

private:
	void ForceOrder(IOrder* order) noexcept;

	bool StartNextExecutableOrder();
	bool IsCrewMemberAbleToExecuteOrder(IOrder* order) noexcept;
	void RemoveIdleOrders() noexcept;
	int ReplaceOrderOfSameType(IOrder* order) noexcept;
	void RemoveAllOrders() noexcept;
	void DeleteRemovedOrders(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept;

private:
	Crewmember* m_pCrewmember;
	std::vector<IOrder*> m_OrderQueue;
	std::vector<IOrder*> m_OrdersToDelete;

	static std::vector<IOrder*> s_OrderCopies[];
};