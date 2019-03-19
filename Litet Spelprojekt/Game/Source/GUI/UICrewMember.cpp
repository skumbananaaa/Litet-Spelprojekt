#include "..\..\Include\GUI\UICrewMember.h"
#include "..\..\Include/Scenes/SceneGame.h"
#include "..\..\Include/Game.h"

UICrewMember::UICrewMember(float x, float y, float width, float height) : Panel(x, y, width, height),
	m_Crewmember(nullptr)
{
	m_TextViewName = new TextView(0, height - 25, width, 25, "Namn", true);
	m_TextViewName->SetBackgroundColor(GUIContext::COLOR_HOVER);
	m_TextViewName->SetTextColor(GUIContext::COLOR_BLACK);

	Add(m_TextViewName);
	Add(new TextView(5, 85, 100, 40, "Roll"));
	Add(new TextView(5, 20, 100, 40, "Skador"));

	SetVisible(false);
}

UICrewMember::~UICrewMember()
{

}

void UICrewMember::SetCrewMember(const Crewmember* crewmember)
{
	if (m_Crewmember != crewmember)
	{
		m_Crewmember = crewmember;
		if(m_Crewmember)
		{
			m_TextViewName->SetText(crewmember->GetName());
		}
		SetVisible(m_Crewmember);
	}
}

void UICrewMember::OnRender(GUIContext* context)
{
	Panel::OnRender(context);
	if (m_Crewmember)
	{
		int32 injuriesOffset = 55;
		static int textureSize = 40;
		std::vector<int32> injuries;
		if (m_Crewmember->HasInjuryBoneBroken())
		{
			injuries.push_back(TEXTURE::ICON_INJURY_BONE);
		}
		if (m_Crewmember->HasInjuryBurned())
		{
			injuries.push_back(TEXTURE::ICON_INJURY_BURN);
		}
		if (m_Crewmember->HasInjurySmoke())
		{
			injuries.push_back(TEXTURE::ICON_INJURY_SMOKE);
		}
		if (m_Crewmember->HasInjuryBleed())
		{
			injuries.push_back(TEXTURE::ICON_INJURY_BLEED);
		}

		static int32 xOffset = 50;

		for (int i = 0; i < injuries.size(); i++)
		{
			context->RenderTexture(ResourceHandler::GetTexture2D(injuries[i]), (i + 1.0f) * injuriesOffset + xOffset, 20.0f, (float)textureSize, (float)textureSize, GUIContext::COLOR_WHITE);
		}
		
		int32 texture = 0;
		switch (m_Crewmember->GetGroupType())
		{
		case SMOKE_DIVER: texture = TEXTURE::ICON_SKILL_FIRE; break;
		case MEDIC: texture = TEXTURE::ICON_SKILL_MEDIC; break;
		default: texture = TEXTURE::ICON_SKILL_STRENGTH; break;
		}
		context->RenderTexture(ResourceHandler::GetTexture2D(texture), injuriesOffset + xOffset, 85.0f, (float)textureSize, (float)textureSize, GUIContext::COLOR_WHITE);
	}
}

void UICrewMember::PrintName() const
{
	std::cout << "UICrewMember" << std::endl;
}