#include "../../Include/Orders/ReplayablePosition.h"
#include "../../Include/Scenes/SceneGame.h"
#include "../../Include/Crewmember.h"

ReplayablePosition::ReplayablePosition(const glm::vec3& position, uint32 shipId) noexcept
{
	RegisterReplayEvent(new std::pair<glm::vec3, uint32>(position, shipId));
}

void ReplayablePosition::BeginReplay(SceneGame* pScene, void* userData) noexcept
{
	std::pair<glm::vec3, uint32>* pair = reinterpret_cast<std::pair<glm::vec3, uint32>*>(userData);
	Crewmember* pCrewMember = pScene->GetCrewmember(pair->second);
	pCrewMember->SetPosition(pair->first);
}
