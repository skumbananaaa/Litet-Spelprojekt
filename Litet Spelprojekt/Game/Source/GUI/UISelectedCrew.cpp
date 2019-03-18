#include "..\..\Include\GUI\UISelectedCrew.h"
#include "..\..\Include/Scenes/SceneGame.h"
#include "..\..\Include/Game.h"

UISelectedCrew::UISelectedCrew(const std::string& name) : ProgressButton(0, 0, 50 + name.length() * 10, 25, name)
{
	SetProgressColor(GUIContext::COLOR_SELECTED);
	SetBackgroundColor(glm::vec4(0.408F, 0.408F, 0.408F, 1.0F));
	SetTextColor(GUIContext::COLOR_BLACK);
	SetOnHoverTextColor(GUIContext::COLOR_BLACK);
	SetPercentage(1.0f);
}

UISelectedCrew::~UISelectedCrew()
{

}

void UISelectedCrew::UpdatePosition(const glm::vec3 & position) noexcept
{
	Camera& camera = Game::GetGame()->m_pSceneGame->GetCamera();
	glm::vec3 crewWorldPos = position + glm::vec3(0.0f, 2.0f, 0.0f);
	crewWorldPos.y = glm::clamp(crewWorldPos.y, std::floor(position.y), std::floor(position.y / 2) * 2 + 1.9f);
	crewWorldPos.x += Game::GetGame()->m_pSceneGame->GetExtension() * glm::floor(crewWorldPos.y / 2.0f);
	glm::vec4 pos = camera.GetCombinedMatrix() * glm::vec4(crewWorldPos, 1);
	pos = pos / pos.w;
	float halfWidth = Game::GetGame()->GetWindow().GetWidth() / 2;
	float halfHeight = Game::GetGame()->GetWindow().GetHeight() / 2;
	float x = pos.x * halfWidth + halfWidth - GetWidth() / 2;
	float y = pos.y * halfHeight + halfHeight + GetHeight() / 2;
	SetPosition(x, y);
}

void UISelectedCrew::PrintName() const
{
	std::cout << "UISelectedCrew" << std::endl;
}
