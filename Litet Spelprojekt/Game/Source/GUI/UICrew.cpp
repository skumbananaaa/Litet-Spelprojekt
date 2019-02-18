#include "..\..\Include\GUI\UICrew.h"
#include "../../Include/Game.h"

UICrew::UICrew(float x, float y, float width, float height, const std::vector<Crewmember*> crewmembers) :
	m_SelectionHandler(false)
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

	static int32 extraHeight = 5;
	static int32 buttonHeight = 35;
	static glm::vec4 buttonColor = glm::vec4(0.25F, 0.25F, 0.25F, 1.0F);
	static glm::vec2 textOffset = glm::vec2(10.0, 0.0);

	m_Fires = new PanelExpandable(x, y + 100, width, 50, fires.size() * buttonHeight + extraHeight, "Rökdykare");
	m_Medics = new PanelExpandable(x, y + 50, width, 50, medics.size() * buttonHeight + extraHeight, "Sjukvårdare");
	m_Strengths = new PanelExpandable(x, y + 0, width, 50, strength.size() * buttonHeight + extraHeight, "Biffar");

	m_Fires->SetUserData(reinterpret_cast<void*>(TEXTURE::ICON_SKILL_FIRE));
	m_Medics->SetUserData(reinterpret_cast<void*>(TEXTURE::ICON_SKILL_MEDIC));
	m_Strengths->SetUserData(reinterpret_cast<void*>(TEXTURE::ICON_SKILL_STRENGTH));

	m_Fires->SetDeleteAllChildrenOnDestruction(true);
	m_Medics->SetDeleteAllChildrenOnDestruction(true);
	m_Strengths->SetDeleteAllChildrenOnDestruction(true);

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
		Button* button = new Button(0, i * buttonHeight + extraHeight, m_Fires->GetWidth(), buttonHeight, fires[i]->GetName());
		button->SetBackgroundColor(buttonColor);
		button->SetTextCentered(false);
		button->SetTextOffset(textOffset);
		button->SetUserData(reinterpret_cast<void*>(fires[i]->GetShipNumber()));
		button->AddButtonListener(this);
		m_HoveringHandler.AddHoverable(button);
		m_Fires->Add(button);
	}

	for (int i = 0; i < medics.size(); i++)
	{
		Button* button = new Button(0, i * buttonHeight + extraHeight, m_Medics->GetWidth(), buttonHeight, medics[i]->GetName());
		button->SetBackgroundColor(buttonColor);
		button->SetTextCentered(false);
		button->SetTextOffset(textOffset);
		button->SetUserData(reinterpret_cast<void*>(medics[i]->GetShipNumber()));
		button->AddButtonListener(this);
		m_HoveringHandler.AddHoverable(button);
		m_Medics->Add(button);
	}

	for (int i = 0; i < strength.size(); i++)
	{
		Button* button = new Button(0, i * buttonHeight + extraHeight, m_Strengths->GetWidth(), buttonHeight, strength[i]->GetName());
		button->SetBackgroundColor(buttonColor);
		button->SetTextCentered(false);
		button->SetTextOffset(textOffset);
		button->SetUserData(reinterpret_cast<void*>(strength[i]->GetShipNumber()));
		button->AddButtonListener(this);
		m_HoveringHandler.AddHoverable(button);
		m_Strengths->Add(button);
	}

	Game::GetGame()->GetGUIManager().Add(m_Fires);
	Game::GetGame()->GetGUIManager().Add(m_Medics);
	Game::GetGame()->GetGUIManager().Add(m_Strengths);
}

UICrew::~UICrew()
{
	Delete(m_Fires);
	Delete(m_Medics);
	Delete(m_Strengths);
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
	Game* game = Game::GetGame();
	game->m_pUICrewMember->SetCrewMember(game->GetCrewmember(reinterpret_cast<uint32>(((Button*)selection)->GetUserData())));
}

void UICrew::OnDehovered(const HoveringHandler* handler, IHoverable* selection)
{
	Game* game = Game::GetGame();
	game->m_pUICrewMember->SetCrewMember(nullptr);
}

void UICrew::OnRenderGUIObject(GUIContext* context, GUIObject* object)
{
	context->RenderTexture(ResourceHandler::GetTexture2D(reinterpret_cast<uint32>(object->GetUserData())), object->GetWidth() - 40, 10, 30, 30, GUIContext::COLOR_WHITE);
}

void UICrew::OnButtonPressed(Button* button)
{
	
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
