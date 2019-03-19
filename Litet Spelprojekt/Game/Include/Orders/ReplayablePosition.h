#pragma once

#include <EnginePch.h>
#include "../IReplayable.h"

class ReplayablePosition : public IReplayable
{
public:
	ReplayablePosition(const glm::vec3& position, uint32 shipId) noexcept;

	virtual void BeginReplay(SceneGame* pScene, void* userData) noexcept override;
	virtual bool IsDeleteable() const noexcept override { return true; }
};