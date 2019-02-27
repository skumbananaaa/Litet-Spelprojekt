#include "..\..\Include\Scenes\SceneMenu.h"
#include "..\..\Include\Scenes\SceneOptions.h"
#include "..\..\Include\Scenes\SceneScenario.h"
#include "..\..\Include\Scenes\SceneCredits.h"
#include "../../Include/Game.h"

SceneMenu::SceneMenu()
{
	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pPanel = new Panel((window->GetWidth() - 600) / 2, (window->GetHeight() - 600) / 2, 600, 600);
	m_pButtonPlay = new Button(100, m_pPanel->GetHeight() - 160, m_pPanel->GetWidth() - 200, 100, "Spela");
	m_pButtonOptions = new Button(100, m_pButtonPlay->GetY() - 125, m_pPanel->GetWidth() - 200, 100, "Alternativ");
	m_pButtonCredits = new Button(100, m_pButtonOptions->GetY() - 125, m_pPanel->GetWidth() - 200, 100, "Utvecklare");
	m_pButtonQuit = new Button(100, m_pButtonCredits->GetY() - 125, m_pPanel->GetWidth() - 200, 100, "Avsluta");

	m_pPanel->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);

	m_pButtonPlay->SetBackgroundColor(glm::vec4(0.0F, 1.0F, 1.0F, 0.9F));
	m_pButtonOptions->SetBackgroundColor(glm::vec4(0.0F, 1.0F, 1.0F, 0.9F));
	m_pButtonCredits->SetBackgroundColor(glm::vec4(0.0F, 1.0F, 1.0F, 0.9F));
	m_pButtonQuit->SetBackgroundColor(glm::vec4(0.0F, 1.0F, 1.0F, 0.9F));

	m_pButtonPlay->SetOnHoverColor(glm::vec4(0.5F, 1.0F, 1.0F, 1.0F));
	m_pButtonOptions->SetOnHoverColor(glm::vec4(0.5F, 1.0F, 1.0F, 1.0F));
	m_pButtonCredits->SetOnHoverColor(glm::vec4(0.5F, 1.0F, 1.0F, 1.0F));
	m_pButtonQuit->SetOnHoverColor(glm::vec4(0.5F, 1.0F, 1.0F, 1.0F));

	m_pButtonPlay->SetOnHoverTextColor(GUIContext::COLOR_BLACK);
	m_pButtonOptions->SetOnHoverTextColor(GUIContext::COLOR_BLACK);
	m_pButtonCredits->SetOnHoverTextColor(GUIContext::COLOR_BLACK);
	m_pButtonQuit->SetOnHoverTextColor(GUIContext::COLOR_BLACK);

	m_pButtonPlay->SetTextColor(GUIContext::COLOR_BLACK);
	m_pButtonOptions->SetTextColor(GUIContext::COLOR_BLACK);
	m_pButtonCredits->SetTextColor(GUIContext::COLOR_BLACK);
	m_pButtonQuit->SetTextColor(GUIContext::COLOR_BLACK);

	m_pButtonPlay->AddExternalRenderer(this);
	m_pButtonOptions->AddExternalRenderer(this);
	m_pButtonCredits->AddExternalRenderer(this);
	m_pButtonQuit->AddExternalRenderer(this);

	m_pButtonPlay->AddButtonListener(this);
	m_pButtonOptions->AddButtonListener(this);
	m_pButtonCredits->AddButtonListener(this);
	m_pButtonQuit->AddButtonListener(this);

	m_pPanel->Add(m_pButtonPlay);
	m_pPanel->Add(m_pButtonOptions);
	m_pPanel->Add(m_pButtonCredits);
	m_pPanel->Add(m_pButtonQuit);

	game->GetGUIManager().Add(m_pPanel);
}

SceneMenu::~SceneMenu()
{
	Game* game = Game::GetGame();
	game->GetGUIManager().DeleteChildren();
}

void SceneMenu::OnUpdate(float dtS) noexcept
{
	SceneInternal::OnUpdate(dtS);
}

void SceneMenu::OnRender(float dtS) noexcept
{
	SceneInternal::OnRender(dtS);
}

void SceneMenu::OnButtonPressed(Button* button)
{
}

void SceneMenu::OnButtonReleased(Button* button)
{
	if (button == m_pButtonPlay)
	{
		Game::GetGame()->SetScene(new SceneScenario());
	}
	else if (button == m_pButtonOptions)
	{
		Game::GetGame()->SetScene(new SceneOptions());
	}
	else if (button == m_pButtonCredits)
	{
		Game::GetGame()->SetScene(new SceneCredits());
	}
	else if (button == m_pButtonQuit)
	{
		Game::GetGame()->Exit();
	}
}

void SceneMenu::OnButtonHovered(Button* button)
{
}

void SceneMenu::OnButtonNotHovered(Button* button)
{
}

void SceneMenu::OnRenderGUIObject(GUIContext* context, GUIObject* object)
{
	static int32 thickness = 3;

	context->RenderTexture(object->GetDefaultTexture(), 0, 0, thickness, object->GetHeight(), GUIContext::COLOR_BLACK);
	context->RenderTexture(object->GetDefaultTexture(), object->GetWidth() - thickness, 0, thickness, object->GetHeight(), GUIContext::COLOR_BLACK);

	context->RenderTexture(object->GetDefaultTexture(), 0, object->GetHeight() - thickness, object->GetWidth(), thickness, GUIContext::COLOR_BLACK);
	context->RenderTexture(object->GetDefaultTexture(), 0, 0, object->GetWidth(), thickness, GUIContext::COLOR_BLACK);
}