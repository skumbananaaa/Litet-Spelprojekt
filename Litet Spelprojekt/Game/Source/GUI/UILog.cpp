#include "..\..\Include\GUI\UILog.h"
#include "../../Include/Game.h"
#include <Audio/Sources/AudioSource.h>

UILog::UILog(float x, float y, float width, float height) : Panel(x, y, width, height), m_Extend(false)
{
	m_pTextViewLog = new TextView(50, height - 50, width - 50, 50, "Loggbok", true);

	m_ListScrollableLog = new ListScrollable(m_pTextViewLog->GetHeight(), 0, width - m_pTextViewLog->GetHeight(), height - m_pTextViewLog->GetHeight());
	m_ListScrollableLog->SetBackgroundColor(glm::vec4(0.15F, 0.15F, 0.15F, 1.0F));

	SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);

	Add(m_pTextViewLog);
	Add(m_ListScrollableLog);
	m_DefaultX = x + width - m_pTextViewLog->GetHeight();

	m_pAudioSourceSelect = AudioSource::CreateSoundSource(SOUND::UI_SELECT);
	m_pAudioSourceSelect->SetRollOffFactor(10.0f);
	m_pAudioSourceSelect->SetReferenceDistance(0.0f);
	m_pAudioSourceSelect->SetMaxDistance(500.0f);
	m_pAudioSourceSelect->SetLooping(false);
}

UILog::~UILog()
{
	DeleteSafe(m_pAudioSourceSelect);
}

void UILog::OnLogged(const std::string& time, const std::string& text) noexcept
{
	glm::vec4 color = m_ListScrollableLog->GetNrOfChildren() % 2 == 0 ? glm::vec4(0.2F, 0.2F, 0.2F, 1.0F) : glm::vec4(0.3F, 0.3F, 0.3F, 1.0F);
	TextView* textView = new TextView(0, 0, m_ListScrollableLog->GetClientWidth(), 40, time + text);
	textView->SetBackgroundColor(color);
	m_ListScrollableLog->Add(textView);

	if (!m_Extend)
	{
		Game::GetGame()->m_pSceneGame->GetUINotification()->CreateNotification(text);
	}

	m_pAudioSourceSelect->Play();
}

void UILog::OnUpdate(float dtS)
{
	m_Percentage += dtS * (m_Extend * 2 - 1) * 5.0F;
	m_Percentage = glm::clamp<float>(m_Percentage, 0.0F, 1.0F);

	SetPosition(m_DefaultX - m_Percentage * (GetWidth() - m_pTextViewLog->GetHeight()), GetY());
}

void UILog::OnRender(GUIContext* context)
{
	Panel::OnRender(context);
	context->RenderTexture(GetDefaultTexture(), 0, GetHeight() - m_pTextViewLog->GetHeight(), GetWidth(), m_pTextViewLog->GetHeight(), GUIContext::COLOR_PANEL_BACKGROUND);
	context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_LOGBOOK), 0, GetHeight() - m_pTextViewLog->GetHeight(), m_pTextViewLog->GetHeight(), m_pTextViewLog->GetHeight(), GUIContext::COLOR_WHITE);
}

void UILog::OnHovered(const glm::vec2& position)
{
	m_Extend = true;
	Game::GetGame()->m_pSceneGame->GetUINotification()->DeleteAll();
}

void UILog::OnNotHovered(const glm::vec2& position)
{
	m_Extend = false;
}

void UILog::PrintName() const
{
	std::cout << "UILog" << std::endl;
}