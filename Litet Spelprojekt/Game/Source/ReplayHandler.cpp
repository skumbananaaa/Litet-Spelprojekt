#include "..\Include\ReplayHandler.h"
#include "../Include/IReplayable.h"
#include "../Include/Scenes/SceneGame.h"

std::queue<std::tuple<IReplayable*, void*, float>> ReplayHandler::s_ReplayQueue;
float ReplayHandler::s_Timer = 0.0F;
bool ReplayHandler::s_IsReplying = false;

void ReplayHandler::Reset() noexcept
{
	s_ReplayQueue = std::queue<std::tuple<IReplayable*, void*, float>>();
	s_Timer = 0.0F;
	s_IsReplying = false;
}

void ReplayHandler::Update(float dtS, SceneGame* pScene) noexcept
{
	s_Timer += dtS;

	if (s_IsReplying)
	{
		while(!s_ReplayQueue.empty())
		{
			auto& tuple = s_ReplayQueue.front();
			if (s_Timer >= std::get<2>(tuple)) //Check if it's time to execute next event
			{
				s_ReplayQueue.pop();
				void* userData = std::get<1>(tuple);
				std::get<0>(tuple)->BeginReplay(pScene, userData); //Execute next event and bundle the userdata

				if (userData)
				{
					delete userData;
				}
			}
			else
			{
				break;
			}
		}	
	}
}

bool ReplayHandler::IsReplaying() noexcept
{
	return s_IsReplying;
}

void ReplayHandler::StartReplay() noexcept
{
	s_Timer = 0.0F;
	s_IsReplying = true;
}

void ReplayHandler::RegisterEvent(IReplayable* pReplayable, void* userData) noexcept
{
	if (!IsReplaying())
	{
		s_ReplayQueue.push(std::make_tuple(pReplayable, userData, s_Timer));
	}
}
