#pragma once

class SceneGame;

class IReplayable
{
public:
	virtual ~IReplayable() {};
	virtual void BeginReplay(SceneGame* pScene, void* userData) noexcept = 0;

protected:
	IReplayable() {};
	void RegisterReplayEvent(void* userData) noexcept;
	static bool IsReplaying() noexcept;
};