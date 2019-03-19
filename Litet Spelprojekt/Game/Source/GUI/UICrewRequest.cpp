#include "../../Include/Game.h"
#include "..\..\Include\GUI\UICrewRequest.h"


UICrewRequest::UICrewRequest(float x, float y, float width, float height) : Panel(x, y, width, height)
{
	Button* pButton = nullptr;

	pButton = new Button(0, 0, 50, 50, "");
	pButton->SetBackgroundTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_RED_DOOR));
	pButton->AddButtonListener(this);
	pButton->SetUserData(reinterpret_cast<void*>(DOOR_COLOR_RED));
	Add(pButton);

	pButton = new Button(50, 0, 50, 50, "");
	pButton->SetBackgroundTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_GREEN_DOOR));
	pButton->AddButtonListener(this);
	pButton->SetUserData(reinterpret_cast<void*>(DOOR_COLOR_GREEN));
	Add(pButton);

	pButton = new Button(100, 0, 50, 50, "");
	pButton->SetBackgroundTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_BLUE_DOOR));
	pButton->AddButtonListener(this);
	pButton->SetUserData(reinterpret_cast<void*>(DOOR_COLOR_BLUE));
	Add(pButton);

	pButton = new Button(150, 0, 50, 50, "");
	pButton->SetBackgroundTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_YELLOW_DOOR));
	pButton->AddButtonListener(this);
	pButton->SetUserData(reinterpret_cast<void*>(DOOR_COLOR_YELLOW));
	Add(pButton);
}

UICrewRequest::~UICrewRequest()
{
}

void UICrewRequest::OnButtonPressed(Button* button)
{
}

void UICrewRequest::OnButtonReleased(Button* button)
{
	uint32 color = (uint32)reinterpret_cast<uint64>(button->GetUserData());
	Game* pGame = Game::GetGame();
	
	pGame->m_pSceneGame->RequestDoorClosed(color);
}

void UICrewRequest::OnButtonHovered(Button* button)
{
}

void UICrewRequest::OnButtonNotHovered(Button* button)
{
}