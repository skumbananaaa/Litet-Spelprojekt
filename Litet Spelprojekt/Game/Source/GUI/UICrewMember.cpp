#include "..\..\Include\GUI\UICrewMember.h"
#include "..\..\Include/Game.h"

UICrewMember::UICrewMember(float x, float y, float width, float height) : Panel(x, y, width, height)
{
	m_TextViewName = new TextView(0, height - 25, width, 25, "Name", CENTER);

	Add(m_TextViewName);
	Add(new TextView(5, 85, 100, 40, "Skills"));
	Add(new TextView(5, 20, 100, 40, "Injuries"));

	SetDeleteAllChildrenOnDestruction(true);
}

UICrewMember::~UICrewMember()
{

}

void UICrewMember::SetCrewMember(const Crewmember* crewmember)
{
	m_Crewmember = crewmember;
	SetVisible(crewmember);

	if (crewmember)
	{
		m_TextViewName->SetText(crewmember->GetName());
		Camera& camera = Game::GetScene()->GetCamera();
		glm::vec4 pos = camera.GetCombinedMatrix() * glm::vec4(crewmember->GetPosition(), 0);
		SetPosition(pos.x, pos.z);
	}
}

void UICrewMember::OnRender(GUIContext* context)
{
	Panel::OnRender(context);

	static int textureSize = 40;

	context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_INJURY_BONE), 75, 20, textureSize, textureSize, GUIContext::COLOR_WHITE);
	context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_INJURY_BURN), 150, 20, textureSize, textureSize, GUIContext::COLOR_WHITE);
	context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_INJURY_SMOKE), 225, 20, textureSize, textureSize, GUIContext::COLOR_WHITE);

	context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_SKILL_FIRE), 75, 85, textureSize, textureSize, GUIContext::COLOR_WHITE);
	context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_SKILL_MEDIC), 150, 85, textureSize, textureSize, GUIContext::COLOR_WHITE);
	context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_SKILL_STRENGTH), 225, 85, textureSize, textureSize, GUIContext::COLOR_WHITE);

	context->GetGraphicsContext()->SetTexture(nullptr, 0);
}

void UICrewMember::PrintName() const
{
	std::cout << "UICrewMember" << std::endl;
}