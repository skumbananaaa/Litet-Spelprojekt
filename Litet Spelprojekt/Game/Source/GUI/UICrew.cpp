#include "..\..\Include\GUI\UICrew.h"
#include "../../Include/Game.h"
#include "../../Include/Scenes/SceneGame.h"
#include <System/Random.h>

UICrew::UICrew(float x, float y, float width, float height, const std::vector<Crewmember*>& crewmembers) :
	m_SelectionHandler(false),
	m_Fires(nullptr),
	m_Medics(nullptr),
	m_Strengths(nullptr)
{
	std::vector<Crewmember*> fires;
	std::vector<Crewmember*> medics;
	std::vector<Crewmember*> strength;

	for (Crewmember* crewmember : crewmembers)
	{
		if (crewmember->GetSkillFire() > crewmember->GetSkillMedic())
		{
			if (crewmember->GetSkillFire() > crewmember->GetSkillStrength())
			{
				fires.push_back(crewmember);
			}
			else if(crewmember->GetSkillMedic() > crewmember->GetSkillStrength())
			{
				medics.push_back(crewmember);
			}
			else
			{
				strength.push_back(crewmember);
			}
		}
		else if (crewmember->GetSkillFire() > crewmember->GetSkillStrength())
		{
			medics.push_back(crewmember);
		}
		else if (crewmember->GetSkillMedic() > crewmember->GetSkillStrength())
		{
			medics.push_back(crewmember);
		}
		else
		{
			strength.push_back(crewmember);
		}
	}

	static int32 buttonHeight = 35;
	static glm::vec4 buttonColor = glm::vec4(GUIContext::COLOR_PANEL_MIDGROUND);
	static glm::vec2 textOffset = glm::vec2(10.0, 0.0);

	m_Fires = new PanelExpandable(x, y + 100, width, 50, fires.size() * buttonHeight, "Rökdykare");
	m_Medics = new PanelExpandable(x, y + 50, width, 50, medics.size() * buttonHeight, "Sjukvårdare");
	m_Strengths = new PanelExpandable(x, y + 0, width, 50, strength.size() * buttonHeight, "Biffar");

	m_Fires->SetUserData(reinterpret_cast<void*>(TEXTURE::ICON_SKILL_FIRE));
	m_Medics->SetUserData(reinterpret_cast<void*>(TEXTURE::ICON_SKILL_MEDIC));
	m_Strengths->SetUserData(reinterpret_cast<void*>(TEXTURE::ICON_SKILL_STRENGTH));

	m_Fires->AddExpandableListener(this);
	m_Medics->AddExpandableListener(this);
	m_Strengths->AddExpandableListener(this);

	m_Fires->AddExternalRenderer(this);
	m_Medics->AddExternalRenderer(this);
	m_Strengths->AddExternalRenderer(this);

	m_Fires->SetTextCentered(false);
	m_Medics->SetTextCentered(false);
	m_Strengths->SetTextCentered(false);

	m_Fires->SetTextOffset(textOffset);
	m_Medics->SetTextOffset(textOffset);
	m_Strengths->SetTextOffset(textOffset);

	m_Fires->SetClientAreaColor(buttonColor);
	m_Medics->SetClientAreaColor(buttonColor);
	m_Strengths->SetClientAreaColor(buttonColor);

	m_SelectionHandler.AddSelectable(m_Fires);
	m_SelectionHandler.AddSelectable(m_Medics);
	m_SelectionHandler.AddSelectable(m_Strengths);
	m_SelectionHandler.AddSelectionListener(this);

	m_HoveringHandler.AddHoveringListener(this);

	for (int i = 0; i < fires.size(); i++)
	{
		m_Fires->Add(CreateButton(fires[i]->GetName(), buttonColor, i * buttonHeight, buttonHeight, textOffset, fires[i]->GetShipNumber()));
	}

	for (int i = 0; i < medics.size(); i++)
	{
		m_Medics->Add(CreateButton(medics[i]->GetName(), buttonColor, i * buttonHeight, buttonHeight, textOffset, medics[i]->GetShipNumber()));
	}

	for (int i = 0; i < strength.size(); i++)
	{
		m_Strengths->Add(CreateButton(strength[i]->GetName(), buttonColor, i * buttonHeight, buttonHeight, textOffset, strength[i]->GetShipNumber()));
	}

	Game::GetGame()->GetGUIManager().Add(m_Fires);
	Game::GetGame()->GetGUIManager().Add(m_Medics);
	Game::GetGame()->GetGUIManager().Add(m_Strengths);
}

UICrew::~UICrew()
{

}

void UICrew::OnExpanding(PanelExpandable* panel, float percentage)
{
	if (panel == m_Fires)
	{
		m_Medics->SetPosition(m_Medics->GetX(), m_Fires->GetYForClientArea() - m_Medics->GetHeight());
		m_Strengths->SetPosition(m_Strengths->GetX(), m_Medics->GetYForClientArea() - m_Strengths->GetHeight());
	}
	else if (panel == m_Medics)
	{
		m_Strengths->SetPosition(m_Strengths->GetX(), m_Medics->GetYForClientArea() - m_Strengths->GetHeight());
	}
}

void UICrew::OnCollapsing(PanelExpandable* panel, float percentage)
{
	if (panel == m_Fires)
	{
		m_Medics->SetPosition(m_Medics->GetX(), m_Fires->GetYForClientArea() - m_Medics->GetHeight());
		m_Strengths->SetPosition(m_Strengths->GetX(), m_Medics->GetYForClientArea() - m_Strengths->GetHeight());
	}
	else if (panel == m_Medics)
	{
		m_Strengths->SetPosition(m_Strengths->GetX(), m_Medics->GetYForClientArea() - m_Strengths->GetHeight());
	}
}

void UICrew::OnSelected(const SelectionHandler* handler, ISelectable* selection)
{

}

void UICrew::OnDeselected(const SelectionHandler* handler, ISelectable* selection)
{

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

ProgressButton* UICrew::CreateButton(const std::string& text, const glm::vec4& color, float y, float height, const glm::vec2& textOffset, int shipnumber)
{
	ProgressButton* button = new ProgressButton(0, y, m_Strengths->GetWidth(), height, text);
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
