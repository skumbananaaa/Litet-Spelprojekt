#pragma once
#include "OrderSleep.h"

class OrderSchedule
{
	friend class OrderExtinguishFire;
public:
	OrderSchedule() = delete;
	~OrderSchedule() = delete;
	OrderSchedule(OrderSchedule&& other) = delete;
	OrderSchedule(const OrderSchedule& other) = delete;
	OrderSchedule& operator=(OrderSchedule&& other) = delete;
	OrderSchedule& operator=(const OrderSchedule& other) = delete;

public:
	static void Init(Scene* pScene);
	static void Release();
	static IOrder* GetIdleOrder();
	static IOrder* GetOrderEat();
	static IOrder* GetOrderSleep();
	static IOrder* GetOrderCook();
	static IOrder* GetOrderWork();
	static IOrder* GetOrderToilet();

private:
	static std::vector<GameObject*> s_Beds;
	static std::vector<GameObject*> s_Toilets;
	static std::vector<GameObject*> s_Instruments;
	static std::vector<GameObject*> s_Chairs;
	static std::vector<GameObject*> s_Ovens;
	static std::vector<GameObject*> s_Extinguishers;
};