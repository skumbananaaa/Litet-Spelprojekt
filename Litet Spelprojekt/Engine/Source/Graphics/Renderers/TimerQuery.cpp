#include <EnginePch.h>
#include <Graphics/Renderers/TimerQuery.h>

TimerQuery::TimerQuery(uint32 numQueries)
{
	NumQueries = numQueries;
	pQueries = new uint32[NumQueries];
	glGenQueries(NumQueries, pQueries);
}

TimerQuery::~TimerQuery()
{
	glDeleteQueries(NumQueries, pQueries);
	DeleteArr(pQueries);
}