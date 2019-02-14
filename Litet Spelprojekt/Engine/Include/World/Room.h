#pragma once
#include <EnginePch.h>

class API Room
{
public:
	Room() noexcept;
	~Room();

private:
	glm::ivec3 m_CenterTile;

};