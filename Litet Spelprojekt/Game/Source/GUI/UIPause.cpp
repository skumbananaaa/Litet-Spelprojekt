#include "..\..\Include\GUI\UIPause.h"
#include "..\..\Include\Game.h"

UIPause::UIPause(float x, float y, float width, float height) : Panel(x, y, width, height)
{
	m_pTextViewTitle = new TextView(0, height - 50, width, 50, "Pausad", true);
	m_pButtonBack = new Button(0, height - 50, 100, 50, "Tillbaka");
	m_pButtonExit = new Button(100, height - 160, width - 200, 100, "Avsluta");

	Add(m_pTextViewTitle);
	Add(m_pButtonBack);
	Add(m_pButtonExit);

	m_pButtonBack->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);
	m_pButtonBack->AddButtonListener(this);

	m_pButtonExit->AddButtonListener(this);

	SetBorderColor(GUIContext::COLOR_BLACK);
	SetBoderThickness(3);
}

UIPause::~UIPause()
{
	m_pButtonExit->RemoveButtonListener(this);
	m_pButtonBack->RemoveButtonListener(this);
}

void UIPause::OnButtonPressed(Button* button)
{
	if (button == m_pButtonExit)
	{
		Game::GetGame()->SetScene(Game::GetGame()->m_pSceneMenu);
	}
	else if (button == m_pButtonBack)
	{
		Game::GetGame()->m_pSceneGame->SetPaused(false);
	}
}

void UIPause::OnButtonReleased(Button* button)
{
}

void UIPause::OnButtonHovered(Button* button)
{
}

void UIPause::OnButtonNotHovered(Button* button)
{
}

void UIPause::PrintName() const
{
	std::cout << "UIPause" << std::endl;
}