#include "..\..\Include\GUI\UIEndScreen.h"
#include "..\..\Include\Game.h"
#include "..\..\Include\GameState.h"
#include "../../Include/Scenes/SceneReplay.h"
#include "../../Include/ReplayHandler.h"
#include "../../Include/Orders/OrderSchedule.h"
#include "../../Include/Scenarios/ScenarioManager.h"
#include <sstream>

UIEndScreen::UIEndScreen(float x, float y, float width, float height, bool lost) : Panel(x, y, width, height)
{
	m_pTextViewTitle = new TextView(0, height - 50, width, 50, "Scenario Slut", true, 90);
	
	m_pTextViewEnd = new TextView(20, height - 120, width - 20, 50, lost ? "Du misslyckades, du är sämst. Vad gör du med ditt liv?" : "Du klarade det, bra jobbat vill du ha en medalj?", true, 80);
	if (lost)
	{
		m_pTextViewEnd->SetTextColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	std::stringstream stream;
	stream.precision(3);
	stream << "Besättning:  " << GameState::GetCrewHealth() * 100.0f << "% av besättningen klarade sig utan skador";
	m_pTextViewCrew = new TextView(20, height - 170, width - 20, 50, stream.str(), false);

	stream.precision(2);
	stream.str("");
	stream << "Bränder:  " << GameState::GetBurningAmount() * 100.0f << "% av skeppet skadades av brand";
	m_pTextViewFire = new TextView(20, height - 220, width - 20, 50, stream.str(), false);

	stream.str("");
	stream << "Vattenläckor:  " << GameState::GetWaterLeakAmount() << " av 3 våningar fylldes med vatten";
	m_pTextViewWater = new TextView(20, height - 270, width - 20, 50, stream.str(), false);

	m_pButtonExit = new Button(width / 2 - (width - 600) - 25, 10, width - 600, 50, "Avsluta");
	m_pButtonReplay = new Button(width / 2 + 25, 10, width - 600, 50, "Repris (WIP)");
	m_pButtonExit->AddButtonListener(this);
	m_pButtonReplay->AddButtonListener(this);

	m_pLog = new ListScrollable((width - (width - 40)) / 2, 70, (width - 40), height - 400);
	m_pLog->SetBackgroundColor(glm::vec4(0.1F, 0.1F, 0.1F, 1.0F));

	Add(m_pTextViewTitle);
	Add(m_pTextViewEnd);
	Add(m_pTextViewCrew);
	Add(m_pTextViewFire);
	Add(m_pTextViewWater);
	Add(m_pButtonExit);
	Add(m_pButtonReplay);
	Add(m_pLog);

	for (int32 i = Logger::GetNumEntries() - 1; i >= 0; i--)
	{
		glm::vec4 color = i % 2 == 0 ? glm::vec4(0.2F, 0.2F, 0.2F, 1.0F) : glm::vec4(0.3F, 0.3F, 0.3F, 1.0F);
		TextView* textView = new TextView(0, 0, m_pLog->GetClientWidth(), 40, Logger::GetEntry(i));
		textView->SetBackgroundColor(color);
		m_pLog->Add(textView);
	}

	SetBorderColor(GUIContext::COLOR_BLACK);
	SetBoderThickness(3);
}

UIEndScreen::~UIEndScreen()
{
	m_pButtonExit->RemoveButtonListener(this);
}

void UIEndScreen::OnButtonPressed(Button* button)
{
	if (button == m_pButtonExit)
	{
		Game::GetGame()->SetScene(Game::GetGame()->m_pSceneMenu);
	}
	else if (button == m_pButtonReplay)
	{
		OrderSchedule::Release();
		ScenarioManager::Reset();
		ResourceHandler::ResetGameObjectCounters();

		if (!ReplayHandler::IsReplaying())
		{
			ReplayHandler::StartReplay();
		}
		else
		{
			ReplayHandler::SoftReset();
		}

		World* pWorld = WorldSerializer::Read("world.json");

		////Enable clipplane for wallmaterial
		ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->SetCullMode(CULL_MODE_NONE);
		ResourceHandler::GetMaterial(MATERIAL::BULKHEADS_STANDARD)->SetCullMode(CULL_MODE_NONE);

		SceneReplay* pSceneReplay = new SceneReplay(pWorld);

		pWorld->Generate(*pSceneReplay);

		pWorld->CopyRoomShadows(*pSceneReplay, Game::GetGame()->m_pSceneGame->GetWorld());

		Game::GetGame()->StartGame(pSceneReplay);
	}
}

void UIEndScreen::OnButtonReleased(Button* button)
{
}

void UIEndScreen::OnButtonHovered(Button* button)
{
}

void UIEndScreen::OnButtonNotHovered(Button* button)
{
}

void UIEndScreen::PrintName() const
{
	std::cout << "UIEndScreen" << std::endl;
}