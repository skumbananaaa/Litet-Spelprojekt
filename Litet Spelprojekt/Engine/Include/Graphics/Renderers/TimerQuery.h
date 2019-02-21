#pragma once
#include <EnginePch.h>

struct API TimerQuery final
{
	TimerQuery(uint32 numQueries);
	~TimerQuery();

	uint32* pQueries = nullptr;
	uint32 NumQueries = 0;
};