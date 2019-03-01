#include "../../Include/Orders/OrderHandler.h"
#include "../../Include/Game.h"

std::vector<IOrder*> OrderHandler::s_OrderQueue;

void OrderHandler::GiveOrder(IOrder* order, Crewmember* crewMember) noexcept
{
	SceneGame* pSceneGame = Game::GetGame()->m_pSceneGame;
	order->m_pCrewMember = crewMember;
	order->StartOrder(pSceneGame, pSceneGame->GetWorld(), pSceneGame->GetCrew());
	s_OrderQueue.push_back(order);
	std::cout << "Order started" << std::endl;
}

void OrderHandler::Update(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	for (int i = s_OrderQueue.size() - 1; i >= 0; i--)
	{
		if (s_OrderQueue[i]->UpdateOrder(pScene, pWorld, pCrewMembers, dtS))
		{
			s_OrderQueue[i]->EndOrder(pScene, pWorld, pCrewMembers);
			DeleteSafe(s_OrderQueue[i]);
			s_OrderQueue.erase(s_OrderQueue.begin() + i);
			std::cout << "Order Ended" << std::endl;
		}
	}
}

void OrderHandler::Release() noexcept
{
	for (IOrder* order : s_OrderQueue)
	{
		DeleteSafe(order);
	}
	s_OrderQueue.clear();
	std::cout << "Orders cleared" << std::endl;

}