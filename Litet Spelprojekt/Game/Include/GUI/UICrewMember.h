#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/TextView.h>
#include "../Crewmember.h"


class UICrewMember : public Panel
{
public:
	UICrewMember(float width, float height);
	virtual ~UICrewMember();

	void SetCrewMember(const Crewmember* crewmember);

protected:
	virtual void OnRender(GUIContext* context) override;
	virtual void PrintName() const override;

private:
	TextView* m_TextViewName;
	const Crewmember* m_Crewmember;
};
