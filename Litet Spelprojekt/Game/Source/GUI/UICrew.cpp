#include "..\..\Include\GUI\UICrew.h"
#include "../../Include/Game.h"
#include "../../Include/Scenes/SceneGame.h"
#include <System/Random.h>

UICrew::UICrew(float x, float y, float width, float height, Crew* crew) :
	m_SelectionHandler(false)
{
	static int32 buttonHeight = 35;
	static glm::vec4 buttonColor = glm::vec4(GUIContext::COLOR_PANEL_MIDGROUND);
	static glm::vec2 textOffset = glm::vec2(10.0, 0.0);


	m_Panels.push_back(CreateExpandable("Sjukvårdare", x, y, width, buttonHeight, TEXTURE::ICON_SKILL_MEDIC, MEDIC, crew, buttonColor, textOffset));
	m_Panels.push_back(CreateExpandable("Rökdykare", x, y, width, buttonHeight, TEXTURE::ICON_SKILL_FIRE, SMOKE_DIVER, crew, buttonColor, textOffset));

	for (int i = m_Panels.size(); i < crew->NrOfSquads(); i++)
	{
		m_Panels.push_back(CreateExpandable("Grupp " + std::to_string(i - 1), x, y, width, buttonHeight, TEXTURE::ICON_SKILL_STRENGTH, i, crew, buttonColor, textOffset));
	}

	m_SelectionHandler.AddSelectionListener(this);
	m_HoveringHandler.AddHoveringListener(this);
}

UICrew::~UICrew()
{

}

void UICrew::OnExpanding(PanelExpandable* panel, float percentage)
{
	bool found = false;
	for (int i = 0; i < m_Panels.size(); i++)
	{
		if (found)
		{
			m_Panels[i]->SetPosition(m_Panels[i]->GetX(), m_Panels[i - 1]->GetYForClientArea() - m_Panels[i]->GetHeight());
		}
		else if (panel == m_Panels[i])
		{
			found = true;
		}
	}
}

void UICrew::OnCollapsing(PanelExpandable* panel, float percentage)
{
	bool found = false;
	for (int i = 0; i < m_Panels.size(); i++)
	{
		if (found)
		{
			m_Panels[i]->SetPosition(m_Panels[i]->GetX(), m_Panels[i - 1]->GetYForClientArea() - m_Panels[i]->GetHeight());
		}
		else if (panel == m_Panels[i])
		{
			found = true;
		}
	}
}

void UICrew::OnSelected(const SelectionHandler* handler, ISelectable* selection)
{
	PanelExpandable* panel = (PanelExpandable*)selection;

	Game::GetGame()->m_pSceneGame->GetCrew()->ClearSelectedList();
	for (int i = 0; i < panel->GetChildren().size(); i++)
	{
		uint32 shipNumber = reinterpret_cast<uint32>(panel->GetChildren()[i]->GetUserData());
		Crewmember* crewmember = Game::GetGame()->m_pSceneGame->GetCrewmember(shipNumber);
		crewmember->SetIsPicked(true);
	}
}

void UICrew::OnDeselected(const SelectionHandler* handler, ISelectable* selection)
{
	Game::GetGame()->m_pSceneGame->GetCrew()->ClearSelectedList();
}

void UICrew::OnHovered(const HoveringHandler* handler, IHoverable* selection)
{
	Game::GetGame()->m_pSceneGame->GetUICrewMember()->SetCrewMember(Game::GetGame()->m_pSceneGame->GetCrewmember(reinterpret_cast<uint32>(((Button*)selection)->GetUserData())));
}

void UICrew::OnDehovered(const HoveringHandler* handler, IHoverable* selection)
{
	Game::GetGame()->m_pSceneGame->GetUICrewMember()->SetCrewMember(nullptr);
}

void UICrew::OnRenderGUIObject(GUIContext* context, GUIObject* object)
{
	context->RenderTexture(ResourceHandler::GetTexture2D(reinterpret_cast<uint32>(object->GetUserData())), object->GetWidth() - 40, 10, 30, 30, GUIContext::COLOR_WHITE);
}

void UICrew::OnButtonPressed(Button* button)
{
	ProgressButton* progressButton = (ProgressButton*)button;
	progressButton->StartAnimation(Random::GenerateInt(3, 15));
	progressButton->SetText("Inväntar svar");
	progressButton->SetTextColor(GUIContext::COLOR_BLACK);
}

void UICrew::OnButtonReleased(Button* button)
{

}

void UICrew::OnButtonHovered(Button* button)
{
	
}

void UICrew::OnButtonNotHovered(Button* button)
{
	
}

void UICrew::OnProgressAnimationEnd(ProgressButton* progressButton)
{
	progressButton->SetPercentage(0.0);
	progressButton->SetTextColor(GUIContext::COLOR_WHITE);
	int32 shipnumber = reinterpret_cast<uint32>(progressButton->GetUserData());
	Game::GetGame()->m_pSceneGame->ShowCrewmember(shipnumber);
	Crewmember* crewmember = Game::GetGame()->m_pSceneGame->GetCrewmember(shipnumber);
	progressButton->SetText(crewmember->GetName());
	crewmember->UpdateLastKnownPosition();
}

void UICrew::SetVisible(bool visible) noexcept
{
	for (int i = 0; i < m_Panels.size(); i++)
	{
		m_Panels[i]->SetVisible(visible);
	}
}

ProgressButton* UICrew::CreateButton(const std::string& text, const glm::vec4& color, float y, float width, float height, const glm::vec2& textOffset, int shipnumber)
{
	ProgressButton* button = new ProgressButton(0, y, width, height, text);
	button->SetBackgroundColor(color);
	button->SetProgressColor(GUIContext::COLOR_SELECTED);
	button->SetTextCentered(false);
	button->SetTextOffset(textOffset);
	button->SetUserData(reinterpret_cast<void*>(shipnumber));
	button->AddButtonListener(this);
	button->AddProgressListener(this);
	m_HoveringHandler.AddHoverable(button);
	return button;
}

PanelExpandable* UICrew::CreateExpandable(const std::string& text, float x, float y, float width, float buttonHeight, uint32 icon, int index, Crew* crew, const glm::vec4& buttonColor, const glm::vec2& textOffset)
{
	PanelExpandable* panel = new PanelExpandable(x, y - index * 50, width, 50, crew->GetSquadCount(index) * buttonHeight, text);
	panel->SetUserData(reinterpret_cast<void*>(icon));
	panel->AddExpandableListener(this);
	panel->AddExternalRenderer(this);
	panel->SetTextCentered(false);
	panel->SetTextOffset(textOffset);
	panel->SetClientAreaColor(buttonColor);
	m_SelectionHandler.AddSelectable(panel);

	for (int i = 0; i < crew->GetSquadCount(index); i++)
	{
		Crewmember* member = crew->GetMember(crew->GetSquad(index)[i]);
		panel->Add(CreateButton(member->GetName(), buttonColor, i * buttonHeight, width, buttonHeight, textOffset, member->GetShipNumber()));
	}

	Game::GetGame()->GetGUIManager().Add(panel);

	return panel;
}
