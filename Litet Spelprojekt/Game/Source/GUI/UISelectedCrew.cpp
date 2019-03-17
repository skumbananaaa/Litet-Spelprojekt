#include "..\..\Include\GUI\UISelectedCrew.h"
#include "..\..\Include/Scenes/SceneGame.h"
#include "..\..\Include/Game.h"

UISelectedCrew::UISelectedCrew(const std::string& name) : TextView(0, 0, 100, 25, name, true)
{
	SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);
	SetTextColor(GUIContext::COLOR_BLACK);

	m_pProgressBar = new ProgressBar(0, 0, 100, 25);
	m_pProgressBar->SetProgressColor(GUIContext::COLOR_SELECTED);
	Game::GetGame()->GetGUIManager().Add(m_pProgressBar);

	m_pProgressBar->SetPercentage(100);
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
	m_pProgressBar->SetPosition(x, y);
}

void UISelectedCrew::SetPercentage(float percentage)
{
	m_pProgressBar->SetPercentage(percentage);
}

void UISelectedCrew::OnRemoved(GUIObject * parent)
{
	Game::GetGame()->GetGUIManager().Remove(m_pProgressBar);
	m_pProgressBar = nullptr;
}

void UISelectedCrew::PrintName() const
{
	std::cout << "UISelectedCrew" << std::endl;
}
