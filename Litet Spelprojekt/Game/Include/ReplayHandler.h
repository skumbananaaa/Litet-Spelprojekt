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
	static void Update(float dtS, SceneGame* pScene) noexcept;
	static bool IsReplaying() noexcept;
	static void StartReplay() noexcept;

private:
	ReplayHandler() {};
	~ReplayHandler() {};

	static void RegisterEvent(IReplayable* pReplayable, void* pUserData) noexcept;

	static std::queue<std::tuple<IReplayable*, void*, float>> s_ReplayQueue;
	static float s_Timer;
	static bool s_IsReplying;
};