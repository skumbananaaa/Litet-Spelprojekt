#include "..\..\Include\GUI\UISelectedCrew.h"
#include "..\..\Include/Scenes/SceneGame.h"
#include "..\..\Include/Game.h"

UISelectedCrew::UISelectedCrew(const std::string& name) : TextView(0, 0, 100, 25, name, true)
{
	SetBackgroundColor(GUIContext::COLOR_SELECTED);
	SetTextColor(GUIContext::COLOR_BLACK);
}

UISelectedCrew::~UISelectedCrew()
{
}

void UISelectedCrew::UpdatePosition(const glm::vec3 & position) noexcept
{
	Camera& camera = Game::GetGame()->m_pSceneGame->GetCamera();
	glm::vec3 crewWorldPos = position + glm::vec3(0.0f, 1.9f, 0.0f);
	crewWorldPos.x += Game::GetGame()->m_pSceneGame->GetExtension() * glm::floor(crewWorldPos.y / 2.0f);
	glm::vec4 pos = camera.GetCombinedMatrix() * glm::vec4(crewWorldPos, 1);
	pos = pos / pos.w;
	float halfWidth = Game::GetGame()->GetWindow().GetWidth() / 2;
	float halfHeight = Game::GetGame()->GetWindow().GetHeight() / 2;
	SetPosition(pos.x * halfWidth + halfWidth - GetWidth() / 2, pos.y * halfHeight + halfHeight + GetHeight() / 2);
}

void UISelectedCrew::PrintName() const
{
	std::cout << "UISelectedCrew" << std::endl;
}
