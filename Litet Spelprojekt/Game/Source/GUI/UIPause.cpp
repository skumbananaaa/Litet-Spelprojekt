#include "..\..\Include\GUI\UIPause.h"
#include "..\..\Include\Game.h"

UIPause::UIPause(float x, float y, float width, float height) : Panel(x, y, width, height)
{
	this->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);

	//Menu Panel
	m_pMenuPanel = new Panel((width - 600) / 2, (height - 600) / 2, 600, 600);
	m_pTextViewTitle = new TextView(0, 600 - 50, 600, 50, "Pausad", true);
	m_pButtonBack = new Button(0, 600 - 50, 100, 50, "Tillbaka");
	m_pButtonTutorial = new Button(100, 600 - 160, 600 - 200, 100, "Tutorial");
	m_pButtonExit = new Button(100, 600 - 270, 600 - 200, 100, "Avsluta");

	m_pMenuPanel->SetBorderColor(GUIContext::COLOR_BLACK);
	m_pMenuPanel->SetBoderThickness(3);
	
	Add(m_pMenuPanel);
	m_pMenuPanel->Add(m_pTextViewTitle);
	m_pMenuPanel->Add(m_pButtonBack);
	m_pMenuPanel->Add(m_pButtonTutorial);
	m_pMenuPanel->Add(m_pButtonExit);

	m_pButtonBack->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);
	m_pButtonBack->AddButtonListener(this);
	m_pButtonTutorial->AddButtonListener(this);
	m_pButtonExit->AddButtonListener(this);

	//Tutorial Panel
	m_TutorialScreenIndex = 0;
	m_TutorialScreens.push_back(TEXTURE::LOADING_CONTROLS);
	m_TutorialScreens.push_back(TEXTURE::TUTORIAL_SCREEN_1);
	m_TutorialScreens.push_back(TEXTURE::TUTORIAL_SCREEN_2);

	m_pTutorialPanel = new Panel(0, 0, width, height);
	m_pButtonNext = new Button(width - 311, 45, 200, 100, "Nästa");
	m_pButtonPrev = new Button(50, 45, 200, 100, "Tillbaka");

	Add(m_pTutorialPanel);
	m_pTutorialPanel->Add(m_pButtonNext);
	m_pTutorialPanel->Add(m_pButtonPrev);

	m_pButtonNext->AddButtonListener(this);
	m_pButtonPrev->AddButtonListener(this);
	m_pTutorialPanel->SetVisible(false);
}

UIPause::~UIPause()
{
	m_pButtonExit->RemoveButtonListener(this);
	m_pButtonBack->RemoveButtonListener(this);
	m_pButtonTutorial->RemoveButtonListener(this);
}

void UIPause::OnButtonPressed(Button* button)
{
	if (m_pMenuPanel->IsVisible())
	{	
		if (button == m_pButtonExit)
		{
			Game::GetGame()->SetScene(Game::GetGame()->m_pSceneMenu);
		}
		else if (button == m_pButtonBack)
		{
			Game::GetGame()->m_pSceneGame->SetPaused(false);
		}
		else if (button == m_pButtonTutorial)
		{
			m_pMenuPanel->SetVisible(false);
			m_pTutorialPanel->SetVisible(true);
			m_TutorialScreenIndex = 0;
			m_pTutorialPanel->SetBackgroundTexture(ResourceHandler::GetTexture2D(m_TutorialScreens[0]));
		}
	}
	else
	{
		if (button == m_pButtonPrev)
		{
			m_TutorialScreenIndex--;

			if (m_TutorialScreenIndex < 0)
			{
				std::cout << "Returning from tutorial" << std::endl;
				m_pMenuPanel->SetVisible(true);
				m_pTutorialPanel->SetVisible(false);
				return;
			}
		}
		else if (button == m_pButtonNext)
		{
			m_TutorialScreenIndex++;
			if (m_TutorialScreenIndex >= m_TutorialScreens.size())
			{
				std::cout << "Exiting tutorial" << std::endl;
				m_pMenuPanel->SetVisible(true);
				m_pTutorialPanel->SetVisible(false);
				return;
			}
		}
		std::cout << "New tutorial texture" << std::endl;
		std::cout << "Texture index: " << m_TutorialScreenIndex << std::endl;
		m_pTutorialPanel->SetBackgroundTexture(ResourceHandler::GetTexture2D(m_TutorialScreens[m_TutorialScreenIndex]));
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