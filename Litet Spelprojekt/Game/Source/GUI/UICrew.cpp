#include "..\..\Include\GUI\UICrew.h"
#include "../../Include/Game.h"

UICrew::UICrew(float x, float y, float width, float height, const std::vector<Crewmember*> crewmembers)
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

	m_Fires = new PanelExpandable(x, 750, width, 50, fires.size() * 30, "Rökdykare");
	m_Medics = new PanelExpandable(x, 700, width, 50, medics.size() * 30, "Sjukvårdare");
	m_Strengths = new PanelExpandable(x, 650, width, 50, strength.size() * 30, "Biffar");

	for (int i = 0; i < fires.size(); i++)
	{
		m_Fires->Add(new Button(0, i * 30, m_Fires->GetWidth(), 30, fires[i]->GetName()));
	}

	for (int i = 0; i < medics.size(); i++)
	{
		m_Medics->Add(new Button(0, i * 30, m_Medics->GetWidth(), 30, medics[i]->GetName()));
	}

	for (int i = 0; i < strength.size(); i++)
	{
		m_Strengths->Add(new Button(0, i * 30, m_Strengths->GetWidth(), 30, strength[i]->GetName()));
	}

	Game::GetGame()->GetGUIManager().Add(m_Fires);
	Game::GetGame()->GetGUIManager().Add(m_Medics);
	Game::GetGame()->GetGUIManager().Add(m_Strengths);
}

UICrew::~UICrew()
{
}