#include "..\Include\ReplayHandler.h"
#include "../Include/IReplayable.h"
#include "../Include/Scenes/SceneGame.h"

std::vector<std::tuple<IReplayable*, void*, float>> ReplayHandler::s_ReplayQueue;
float ReplayHandler::s_Timer = 0.0F;
float ReplayHandler::s_TotalTime = 0.0F;
bool ReplayHandler::s_IsReplying = false;
int ReplayHandler::s_Index = 0;

void ReplayHandler::Reset() noexcept
{
	for (uint32 i = 0; i < s_ReplayQueue.size(); i++)
	{
		if (std::get<0>(s_ReplayQueue[i]) != nullptr)
		{
			std::get<0>(s_ReplayQueue[i])->DestroyUserData(std::get<1>(s_ReplayQueue[i]));

			if (std::get<0>(s_ReplayQueue[i])->IsDeleteable())
			{
				DeleteSafe(std::get<0>(s_ReplayQueue[i]));
			}
		}
	}

	s_ReplayQueue.clear();
	s_Timer = 0.0F;
	s_Index = 0;
	s_IsReplying = false;
}

void ReplayHandler::Update(float dtS, SceneGame* pScene) noexcept
{
	if (s_IsReplying)
	{
		while(s_Index < s_ReplayQueue.size())
		{
			std::tuple<IReplayable*, void*, float>& tuple = s_ReplayQueue[s_Index];
			if (s_Timer >= std::get<2>(tuple)) //Check if it's time to execute next event
			{
				s_Index++;
				void* userData = std::get<1>(tuple);
				std::get<0>(tuple)->BeginReplay(pScene, userData); //Execute next event and bundle the userdata
			}
			else
			{
				break;
			}
		}	
	}

	s_Timer += dtS;
}

bool ReplayHandler::IsReplaying() noexcept
{
	return s_IsReplying;
}

void ReplayHandler::StartReplay() noexcept
{
	s_TotalTime = s_Timer;
	SoftReset();
}

void ReplayHandler::SoftReset() noexcept
{
	s_Timer = 0.0F;
	s_Index = 0;
	s_IsReplying = true;
}

float ReplayHandler::GetTotalTime() noexcept
{
	return s_TotalTime;
}

float ReplayHandler::GetElapsedTime() noexcept
{
	return s_Timer;
}

void ReplayHandler::RegisterEvent(IReplayable* pReplayable, void* userData) noexcept
{
	if (!IsReplaying())
	{
		s_ReplayQueue.push_back(std::make_tuple(pReplayable, userData, s_Timer));
	}
}
