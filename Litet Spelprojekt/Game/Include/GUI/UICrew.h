#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/PanelExpandable.h>
#include "../Crewmember.h"

class UICrew
{
public:
	UICrew(float x, float y, float width, float height, const std::vector<Crewmember*> crewmembers);
	virtual ~UICrew();

private:
	PanelExpandable* m_Fires;
	PanelExpandable* m_Medics;
	PanelExpandable* m_Strengths;
};
