#pragma once
#include <queue>
#include <tuple>

class IReplayable;
class SceneGame;

class ReplayHandler
{
	friend class IReplayable;

public:
	
	static void Reset() noexcept;
	__declspec(noinline) static void Update(float dtS, SceneGame* pScene) noexcept;
	static bool IsReplaying() noexcept;
	static void StartReplay() noexcept;
	static void I_DONT_KNOW_WHAT_TO_NAME_THIS_FUNKTION() noexcept;
	static float GetTotalTime() noexcept;
	static float GetElapsedTime() noexcept;

private:
	ReplayHandler() {};
	~ReplayHandler() {};

	static void RegisterEvent(IReplayable* pReplayable, void* pUserData) noexcept;

	static std::vector<std::tuple<IReplayable*, void*, float>> s_ReplayQueue;
	static float s_Timer;
	static float s_TotalTime;
	static bool s_IsReplying;
	static int s_Index;
};