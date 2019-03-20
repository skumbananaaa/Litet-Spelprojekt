#include "../Include/IReplayable.h"
#include "../Include/ReplayHandler.h"

void IReplayable::RegisterReplayEvent(void* userData) noexcept
{
	ReplayHandler::RegisterEvent(this, userData);
}

bool IReplayable::IsReplaying() noexcept
{
	return ReplayHandler::IsReplaying();
}
