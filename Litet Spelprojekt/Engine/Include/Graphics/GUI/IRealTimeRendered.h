#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/GUIContext.h>

class API IRealTimeRendered
{
public:
	virtual void RenderRealTime(GUIContext* context) = 0;
};