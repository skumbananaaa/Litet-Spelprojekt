#include "..\..\Include\GUI\UICrewMember.h"
#include "..\..\Include/Scenes/SceneGame.h"
#include "..\..\Include/Game.h"

UICrewMember::UICrewMember(float width, float height) : Panel(0, 0, width, height),
	m_Crewmember(nullptr)
{
	m_TextViewName = new TextView(0, height - 25, width, 25, "Name", true);
	m_TextViewName->SetBackgroundColor(GUIContext::COLOR_HOVER);
	m_TextViewName->SetTextColor(GUIContext::COLOR_BLACK);

	Add(m_TextViewName);
	Add(new TextView(5, 85, 100, 40, "Färdigheter"));
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
			Camera& camera = Game::GetGame()->m_pSceneGame->GetCamera();
			glm::vec3 crewWorldPos = crewmember->GetLastKnownPosition() + glm::vec3(0.0f, 0.9f, 0.0f);
			crewWorldPos.x += Game::GetGame()->m_pSceneGame->GetExtension() * glm::floor(crewWorldPos.y / 2.0f);
			glm::vec4 pos = camera.GetCombinedMatrix() * glm::vec4(crewWorldPos, 1);
			pos = pos / pos.w;
			float halfWidth = Game::GetGame()->GetWindow().GetWidth() / 2;
			float halfHeight = Game::GetGame()->GetWindow().GetHeight() / 2;
			SetPosition(pos.x * halfWidth + halfWidth - GetWidth() / 2, pos.y * halfHeight + halfHeight + GetHeight() / 2);
		}
		SetVisible(m_Crewmember);
	}
}

void UICrewMember::OnRender(GUIContext* context)
{
	Panel::OnRender(context);
	if (m_Crewmember)
	{
		int32 injuriesOffset = 75;
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

		if (injuries.size() > 3)
		{
			injuriesOffset = 55;
		}

		for (int i = 0; i < injuries.size(); i++)
		{
			context->RenderTexture(ResourceHandler::GetTexture2D(injuries[i]), (i + 1) * injuriesOffset + xOffset, 20, textureSize, textureSize, GUIContext::COLOR_WHITE);
		}

		glm::vec4 colors[3] = { GUIContext::COLOR_GREEN, GUIContext::COLOR_BLUE, GUIContext::COLOR_PURPLE };

		context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_SKILL_FIRE), 75 + xOffset, 85, textureSize, textureSize, GUIContext::COLOR_WHITE);
		context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_SKILL_MEDIC), 150 + xOffset, 85, textureSize, textureSize, GUIContext::COLOR_WHITE);
		context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_SKILL_STRENGTH), 225 + xOffset, 85, textureSize, textureSize, GUIContext::COLOR_WHITE);
		/*context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_CIRCLE), 65 + xOffset, 75, 20, 20, colors[m_Crewmember->GetSkillFire() - 1]);
		context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_CIRCLE), 140 + xOffset, 75, 20, 20, colors[m_Crewmember->GetSkillMedic() - 1]);
		context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_CIRCLE), 215 + xOffset, 75, 20, 20, colors[m_Crewmember->GetSkillStrength() - 1]);
		context->RenderText(std::to_string(m_Crewmember->GetSkillFire()), 75 + xOffset, 85, GetWidth(), GetHeight(), 0.75F, GUIContext::COLOR_BLACK, CENTER);
		context->RenderText(std::to_string(m_Crewmember->GetSkillMedic()), 150 + xOffset, 85, GetWidth(), GetHeight(), 0.75F, GUIContext::COLOR_BLACK, CENTER);
		context->RenderText(std::to_string(m_Crewmember->GetSkillStrength()), 225 + xOffset, 85, GetWidth(), GetHeight(), 0.75F, GUIContext::COLOR_BLACK, CENTER);*/
	}
}

void UICrewMember::PrintName() const
{
	std::cout << "UICrewMember" << std::endl;
}