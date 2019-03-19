#pragma once

class SceneGame;

class IReplayable
{
public:
	virtual ~IReplayable() {};
	virtual void BeginReplay(SceneGame* pScene, void* userData) noexcept = 0;
	virtual bool IsDeleteable() const noexcept { return false; }

protected:
	IReplayable() {};
	void RegisterReplayEvent(void* userData) noexcept;
	static bool IsReplaying() noexcept;
};