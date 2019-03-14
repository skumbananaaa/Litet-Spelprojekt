#include "..\..\Include\GUI\UINotification.h"
#include <Graphics/GUI/TextView.h>
#include "../../Include/Game.h"

UINotification::UINotification(float x, float y, float width, float height, float time) : m_Bounds(x, y, width, height)
{
	m_Time = time;
	m_PixelsToMove = 0;

	m_pAudioSourceSelect = AudioSource::CreateSoundSource(SOUND::UI_SELECT);
	m_pAudioSourceSelect->SetRollOffFactor(10.0f);
	m_pAudioSourceSelect->SetReferenceDistance(0.0f);
	m_pAudioSourceSelect->SetMaxDistance(500.0f);
	m_pAudioSourceSelect->SetLooping(false);
}

UINotification::~UINotification()
{
	DeleteSafe(m_pAudioSourceSelect);
}

void UINotification::Update(float dtS) noexcept
{
	float pixels = 0.0;
	if (m_PixelsToMove > 0)
	{
		pixels = dtS * 200.0F;
	}

	for (int i = m_Notifications.size() - 1; i >= 0; i--)
	{
		std::pair<Panel*, float>& pair = m_Notifications[i];
		Panel* p = pair.first;
		pair.second -= dtS;
		p->SetPosition(p->GetX(), p->GetY() - pixels);
		if (pair.second <= 0)
		{
			m_NotificationsToDelete.push_back(pair.first);
			m_Notifications.erase(m_Notifications.begin() + i);
		}
	}

	m_PixelsToMove -= pixels;
	m_PixelsToMove = glm::max(m_PixelsToMove, 0.0F);


	for (int i = m_NotificationsToDelete.size() - 1; i >= 0; i--)
	{
		Panel* pPanel = m_NotificationsToDelete[i];
		pPanel->SetPosition(pPanel->GetX() + dtS * 400.0f, pPanel->GetY());
		if (pPanel->GetX() > Game::GetGame()->GetWindow().GetWidth())
		{
			pPanel->RemoveExternalRenderer(this);
			Game::GetGame()->GetGUIManager().Remove(pPanel);
			m_NotificationsToDelete.erase(m_NotificationsToDelete.begin() + i);
		}
	}
}

void UINotification::OnRenderGUIObject(GUIContext* context, GUIObject* object)
{
	context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_NOTIFICATION_TRIANGLE), 0,0, object->GetHeight(), object->GetHeight(), GUIContext::COLOR_WHITE);
}

void UINotification::DeleteAll() noexcept
{
	for (int i = m_Notifications.size() - 1; i >= 0; i--)
	{
		Game::GetGame()->GetGUIManager().Remove(m_Notifications[i].first);
		m_Notifications.erase(m_Notifications.begin() + i);
	}

	for (int i = m_NotificationsToDelete.size() - 1; i >= 0; i--)
	{
		Game::GetGame()->GetGUIManager().Remove(m_NotificationsToDelete[i]);
		m_NotificationsToDelete.erase(m_NotificationsToDelete.begin() + i);
	}
}

void UINotification::CreateNotification(const std::string& text)
{
	float y = m_Bounds.y;
	if (!m_Notifications.empty())
	{
		y = m_Notifications[m_Notifications.size() - 1].first->GetY() + 60;
	}

	Panel* pPanel = new Panel(m_Bounds.x, y, m_Bounds.z, m_Bounds.w);
	pPanel->AddExternalRenderer(this);
	pPanel->Add(new TextView(50, 0, pPanel->GetWidth() - 50, 50, text));

	m_Notifications.push_back(std::pair<Panel*, float>(pPanel, m_Time));
	Game::GetGame()->GetGUIManager().Add(pPanel);

	m_PixelsToMove += 60;

	m_pAudioSourceSelect->Play();
}
