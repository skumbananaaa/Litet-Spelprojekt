#pragma once
#include <EnginePch.h>
#include <GLM/glm.hpp>

class API AudioListener
{
public:
	static void SetPosition(const glm::vec3& pos) noexcept;
};