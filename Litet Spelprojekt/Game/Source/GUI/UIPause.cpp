#include "..\..\Include\GUI\UIPause.h"
#include "..\..\Include\Game.h"
#include "../../Include/GUI/UIOptions.h"

UIPause::UIPause(float x, float y, float width, float height) : Panel(x, y, width, height)
{
	this->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);

	//Menu Panel
	m_pMenuPanel = new Panel((width - 600) / 2, (height - 600) / 2, 600, 600);
	m_pTextViewTitle = new TextView(0, 600 - 50, 600, 50, "Pausad", true);
	m_pButtonBack = new Button(0, 600 - 50, 100, 50, "Tillbaka");
	m_pButtonTutorial = new Button(100, 600 - 175, 600 - 200, 100, "Tutorial");
	m_pButtonOptions = new Button(100, 600 - 300, 600 - 200, 100, "Alternativ");
	m_pButtonExit = new Button(100, 600 - 425, 600 - 200, 100, "Avsluta");

	m_pMenuPanel->SetBorderColor(GUIContext::COLOR_BLACK);
	m_pMenuPanel->SetBoderThickness(3);
	
	m_pMenuPanel->Add(m_pTextViewTitle);
	m_pMenuPanel->Add(m_pButtonBack);
	m_pMenuPanel->Add(m_pButtonTutorial);
	m_pMenuPanel->Add(m_pButtonOptions);
	m_pMenuPanel->Add(m_pButtonExit);

	m_pButtonBack->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);
	m_pButtonBack->AddButtonListener(this);
	m_pButtonTutorial->AddButtonListener(this);
	m_pButtonOptions->AddButtonListener(this);
	m_pButtonExit->AddButtonListener(this);

	//Tutorial Panel
	m_TutorialScreenIndex = 0;
	m_TutorialScreens.push_back(TEXTURE::LOADING_CONTROLS);
	m_TutorialScreens.push_back(TEXTURE::TUTORIAL_SCREEN_1);
	m_TutorialScreens.push_back(TEXTURE::TUTORIAL_SCREEN_2);
	m_TutorialScreens.push_back(TEXTURE::TUTORIAL_SCREEN_3);

	m_pTutorialPanel = new Panel(0, 0, width, height);
	m_pButtonNext = new Button(width - 311, 45, 200, 100, "Nästa");
	m_pButtonPrev = new Button(50, 45, 200, 100, "Tillbaka");

	m_pTutorialPanel->Add(m_pButtonNext);
	m_pTutorialPanel->Add(m_pButtonPrev);

	m_pButtonNext->AddButtonListener(this);
	m_pButtonPrev->AddButtonListener(this);
	m_pTutorialPanel->SetVisible(false);

	//Options Panel
	m_pOptionsPanel = new Panel((width - 600) / 2, (height - 600) / 2, 600, 600);
	m_pTextViewTitleOptions = new TextView(0, 600 - 50, 600, 50, "Alternativ", true);
	m_pButtonBackOptions = new Button(0, 600 - 50, 100, 50, "Tillbaka");
	m_pUIOptions = new UIOptions(3, 3, m_pOptionsPanel->GetWidth() - 6, m_pButtonBackOptions->GetY() - 3);

	m_pOptionsPanel->SetVisible(false);
	m_pOptionsPanel->SetBorderColor(GUIContext::COLOR_BLACK);
	m_pOptionsPanel->SetBoderThickness(3);

	m_pButtonBackOptions->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);
	m_pButtonBackOptions->AddButtonListener(this);

	m_pOptionsPanel->Add(m_pTextViewTitleOptions);
	m_pOptionsPanel->Add(m_pButtonBackOptions);
	m_pOptionsPanel->Add(m_pUIOptions);



	Add(m_pTutorialPanel);
	Add(m_pOptionsPanel);
	Add(m_pMenuPanel);
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
			m_pOptionsPanel->SetVisible(false);
			m_pMenuPanel->SetVisible(false);
			m_pTutorialPanel->SetVisible(true);
			m_TutorialScreenIndex = 0;
			m_pTutorialPanel->SetBackgroundTexture(ResourceHandler::GetTexture2D(m_TutorialScreens[0]));
		}
		else if (button == m_pButtonOptions)
		{
			m_pOptionsPanel->SetVisible(true);
			m_pMenuPanel->SetVisible(false);
			m_pTutorialPanel->SetVisible(false);
		}
	}
	else if(m_pTutorialPanel->IsVisible())
	{
		if (button == m_pButtonPrev)
		{
			m_TutorialScreenIndex--;

			if (m_TutorialScreenIndex < 0)
			{
				std::cout << "Returning from tutorial" << std::endl;
				m_pMenuPanel->SetVisible(true);
				m_pTutorialPanel->SetVisible(false);
				m_pOptionsPanel->SetVisible(false);
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
				m_pOptionsPanel->SetVisible(false);
				return;
			}
		}
		std::cout << "New tutorial texture" << std::endl;
		std::cout << "Texture index: " << m_TutorialScreenIndex << std::endl;
		m_pTutorialPanel->SetBackgroundTexture(ResourceHandler::GetTexture2D(m_TutorialScreens[m_TutorialScreenIndex]));
	}
	else if (m_pOptionsPanel->IsVisible())
	{
		if (button == m_pButtonBackOptions)
		{
			m_pMenuPanel->SetVisible(true);
			m_pTutorialPanel->SetVisible(false);
			m_pOptionsPanel->SetVisible(false);
		}
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